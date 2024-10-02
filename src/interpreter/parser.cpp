//
// Created by eric on 9/29/24.
//

#include "parser.h"

#include <csignal>
#include <cstring>

namespace cpplox {

    std::optional<token> parser::next_is(std::span<const token_type> types)
    {
        if(m_scanner.is_eof()) return std::nullopt;
        auto tk = peek();
        if(std::ranges::find(types, tk.type) != types.end())
        {
            get();
            return tk;
        }
        return std::nullopt;
    }
    token parser::get()
    {
        if(peek_buffer.empty())
        {
            return m_scanner.scan();
        }
        auto data = peek_buffer.front();
        peek_buffer.pop();
        return data;
    }
    void parser::synchronize()
    {
        get();
        while(!m_scanner.is_eof())
        {
            switch(peek().type)
            {
            using enum token_type;
            case Class: [[fallthrough]];
            case Fun: [[fallthrough]];
            case Var: [[fallthrough]];
            case For: [[fallthrough]];
            case If: [[fallthrough]];
            case While: [[fallthrough]];
            case Print: [[fallthrough]];
            case Return: [[fallthrough]];
                return;
            default:;
            }

            get();
        }
    }

    token parser::peek()
    {
        if(peek_buffer.empty())
        {
            peek_buffer.push(m_scanner.scan());
        }
        return peek_buffer.front();
    }


    using enum token_type;
    std::unique_ptr<statement> parser::parse()
    {
        return stat();
    }
    std::unique_ptr<statement> parser::stat()
    {
        auto tk = next_is({{Print}});
        if(tk) return print_stat();
        return expr_stat();
    }

    std::unique_ptr<statement> parser::print_stat()
    {
        auto exp = expr();
        auto sc = get();
        if(sc.type != Semicolon) throw parse_error("Expected ';' after expression", sc.line);
        auto st = std::make_unique<print_statement>();
        st->expr = std::move(exp);
        return st;
    }

    std::unique_ptr<statement> parser::expr_stat()
    {
        auto exp = expr();
        auto sc = get();
        if(sc.type != Semicolon) throw parse_error("Expected ';' after expression", sc.line);
        auto st = std::make_unique<expression_statement>();
        st->expr = std::move(exp);
        return st;
    }

    std::unique_ptr<expression> parser::expr()
    {
        return ternary();
    }
    std::unique_ptr<expression> parser::ternary()
    {
        auto condition = equality();
        auto tk = next_is({{Question}});
        if(tk.has_value())
        {
            auto new_expr = std::make_unique<ternary_expression>();
            auto valid = expr();
            auto colon = get();
            if(colon.type != Colon)
                throw parse_error("Expected ':' in ternary expression", colon.line);
            new_expr->condition = std::move(condition);
            new_expr->valid = std::move(valid);
            new_expr->invalid = ternary();
            return new_expr;
        }
        return condition;
    }
    std::unique_ptr<expression> parser::equality()
    {
        auto expr = comparison();
        auto tk = next_is({{ExclamationEqual, DoubleEqual}});
        while(tk.has_value())
        {
            auto new_expr = std::make_unique<binary_expression>();
            new_expr->left = std::move(expr);
            new_expr->right = comparison();
            new_expr->operator_token = *tk;
            expr = std::move(new_expr);
            tk = next_is({{ExclamationEqual, DoubleEqual}});
        }
        return expr;
    }
    std::unique_ptr<expression> parser::comparison()
    {
        auto expr = term();
        auto tk = next_is({{Greater, GreaterEqual, Less, LessEqual}});
        while(tk.has_value())
        {
            auto new_expr = std::make_unique<binary_expression>();
            new_expr->left = std::move(expr);
            new_expr->right = term();
            new_expr->operator_token = *tk;
            expr = std::move(new_expr);
            tk = next_is({{Greater, GreaterEqual, Less, LessEqual}});
        }
        return expr;
    }
    std::unique_ptr<expression> parser::term()
    {
        auto expr = factor();
        auto tk = next_is({{Plus, Minus}});
        while(tk.has_value())
        {
            auto new_expr = std::make_unique<binary_expression>();
            new_expr->left = std::move(expr);
            new_expr->right = factor();
            new_expr->operator_token = *tk;
            expr = std::move(new_expr);
            tk = next_is({{Plus, Minus}});
        }
        return expr;
    }
    std::unique_ptr<expression> parser::factor()
    {
        auto expr = unary();
        auto tk = next_is({{Slash, Star}});
        while(tk.has_value())
        {
            auto new_expr = std::make_unique<binary_expression>();
            new_expr->left = std::move(expr);
            new_expr->right = unary();
            new_expr->operator_token = *tk;
            expr = std::move(new_expr);
            tk = next_is({{Plus, Minus}});
        }
        return expr;
    }
    std::unique_ptr<expression> parser::unary()
    {
        if(auto tk = next_is({{Exclamation, Minus}}))
        {
            auto new_expr = std::make_unique<unary_expression>();
            new_expr->operator_token = *tk;
            new_expr->operand = unary();
            return new_expr;
        }
        return primary();
    }
    std::unique_ptr<expression> parser::primary()
    {
        auto tk = get();
        if(tk.type == NumericLiteral)
        {
            double val = std::stod(tk.text);
            auto expr = std::make_unique<literal_expression>();
            expr->value = val;
            return expr;
        }
        if(tk.type == StringLiteral)
        {
            auto expr = std::make_unique<literal_expression>();
            expr->value = tk.text;
            return expr;
        }
        if(tk.type == True)
        {
            auto expr = std::make_unique<literal_expression>();
            expr->value = true;
            return expr;
        }
        if(tk.type == False)
        {
            auto expr = std::make_unique<literal_expression>();
            expr->value = false;
            return expr;
        }
        if(tk.type == Nil)
        {
            auto expr = std::make_unique<literal_expression>();
            expr->value = std::monostate{};
            return expr;
        }
        if(tk.type == LParen)
        {
            auto exp = expr();
            auto grp = std::make_unique<group_expression>();
            grp->operand = std::move(exp);
            auto rparen = get();
            if(rparen.type != token_type::RParen) throw parse_error("Expected ')' after expression", rparen.line);
            return grp;
        }
        throw parse_error("Expected expression", tk.line);
    }



} // cpplox