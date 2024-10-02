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
        return declaration();
    }
    std::unique_ptr<statement> parser::declaration()
    {
        try
        {
            if(auto tk = next_is({{Var}}))
                return variable_declaration();
            return stat();
        }
        catch (parse_error& e)
        {
            synchronize();
            throw;
        }
    }
    std::unique_ptr<statement> parser::variable_declaration()
    {
        std::unique_ptr<variable_statement> res = std::make_unique<variable_statement>();
        auto iden = get();
        if(iden.type != Identifier) throw parse_error("Expected identifier after expression", iden.line);
        res->name = std::move(iden);
        if(next_is({{Equal}})) res->init = expr();
        auto sc = get();
        if(sc.type != Semicolon) throw parse_error("Expected ';' after expression", sc.line);
        return res;
    }
    std::unique_ptr<statement> parser::stat()
    {
        if(auto tk = next_is({{Print}})) return print_stat();
        if(auto tk = next_is({{LBrace}})) return block_stat();
        if(auto tk = next_is({{If}})) return if_stat();
        return expr_stat();
    }
    std::unique_ptr<statement> parser::if_stat()
    {
        auto st = std::make_unique<if_statement>();
        st->condition = expr();
        st->then_statement = stat();
        if(auto tk = next_is({{Else}})) st->else_statement = stat();
        return st;
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

    std::unique_ptr<statement> parser::block_stat()
    {
        std::unique_ptr<block_statement> res = std::make_unique<block_statement>();
        auto tk = next_is({{RBrace}});
        while(!tk && !m_scanner.is_eof())
        {
            res->statements.emplace_back(declaration());
            tk = next_is({{RBrace}});
        }
        if(tk->type != RBrace) throw parse_error("Expected '}' after block", tk->line);
        return res;
    }

    std::unique_ptr<expression> parser::expr()
    {
        return assignment();
    }
    std::unique_ptr<expression> parser::assignment()
    {
        auto exp = ternary();
        if(auto tk = next_is({{Equal}}))
        {
            auto value = assignment();
            if(auto p = dynamic_cast<variable_expression*>(exp.get()))
            {
                auto assign = std::make_unique<assignment_expression>();
                assign->name = p->name;
                assign->value = std::move(value);
                return assign;
            }
            throw parse_error("Invalid assignment target", tk->line);
        }
        return exp;
    }

    std::unique_ptr<expression> parser::ternary()
    {
        auto condition = or_expr();
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
    std::unique_ptr<expression> parser::or_expr()
    {
        auto left = and_expr();
        auto tk = next_is({{Or}});
        while(tk)
        {
            auto new_expr = std::make_unique<logical_expression>();
            auto rhs = and_expr();
            new_expr->left = std::move(left);
            new_expr->right = std::move(rhs);
            new_expr->tk = *tk;
            left = std::move(new_expr);
            tk = next_is({{Or}});
        }
        return left;
    }
    std::unique_ptr<expression> parser::and_expr()
    {
        auto left = equality();
        auto tk = next_is({{And}});
        while(tk)
        {
            auto new_expr = std::make_unique<logical_expression>();
            auto rhs = equality();
            new_expr->left = std::move(left);
            new_expr->right = std::move(rhs);
            new_expr->tk = *tk;
            left = std::move(new_expr);
            tk = next_is({{And}});
        }
        return left;
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
        if(tk.type == Identifier)
        {
            auto expr = std::make_unique<variable_expression>();
            expr->name = tk;
            return expr;
        }
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