#pragma once
#include <iostream>

#include "expression.h"
#include "statement.h"
#include "environment.h"
namespace cpplox
{
    class execution_error : public std::exception
    {
        std::string message;
        int line_number;
    public:
        [[nodiscard]] const char* what() const noexcept override
        {
            return message.c_str();
        }
        [[nodiscard]] int line_no() const noexcept
        {
            return line_number;
        }
        execution_error(std::string message, int line) :message(std::move(message)), line_number(line) {}
    };
    class executor {
    public:
        lox_type operator()(ternary_expression* exr)
        {
            auto cond = std::visit(*this, to_variant(exr->condition.get()));
            if(is_truthy(cond))
            {
                return std::visit(*this, to_variant(exr->valid.get()));
            }
            return std::visit(*this, to_variant(exr->invalid.get()));
        }
        lox_type operator()(binary_expression* exr)
        {
            auto left = std::visit(*this, to_variant(exr->left.get()));
            auto right = std::visit(*this, to_variant(exr->right.get()));
            int line = exr->operator_token.line;
            switch (exr->operator_token.type)
            {
            case token_type::Minus:
                check_number_operands(line, left, right);
                return std::get<double>(left) - std::get<double>(right);
            case token_type::Star:
                check_number_operands(line, left, right);
                return std::get<double>(left) * std::get<double>(right);
            case token_type::Slash:
                check_number_operands(line, left, right);
                return std::get<double>(left) / std::get<double>(right);
            case token_type::Plus:
                if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return std::get<double>(left) + std::get<double>(right);
                if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) return std::get<std::string>(left) + std::get<std::string>(right);
            case token_type::Greater:
                check_number_operands(line, left, right);
                return std::get<double>(left) > std::get<double>(right);
            case token_type::Less:
                check_number_operands(line, left, right);
                return std::get<double>(left) < std::get<double>(right);
            case token_type::LessEqual:
                check_number_operands(line, left, right);
                return std::get<double>(left) <= std::get<double>(right);
            case token_type::GreaterEqual:
                check_number_operands(line, left, right);
                return std::get<double>(left) >= std::get<double>(right);
            case token_type::DoubleEqual: return left == right;
            case token_type::ExclamationEqual: return left != right;
            }
        };
        lox_type operator()(unary_expression* exr)
        {
            auto right = std::visit(*this, to_variant(exr->operand.get()));

            switch(exr->operator_token.type)
            {
            case token_type::Minus: return -std::get<double>(right);
            case token_type::Exclamation: return  !is_truthy(right);
            }
        }
        lox_type operator()(literal_expression* exr) const
        {
            return exr->value;
        }
        lox_type operator()(group_expression* exr)
        {
            return std::visit(*this, to_variant(exr->operand.get()));
        }
        lox_type operator()(variable_expression* exr) const
        {
            return env.get(exr->name);
        }
        lox_type operator()(assignment_expression* exr)
        {
            auto val = std::visit(*this, to_variant(exr->value.get()));
            env.assign(exr->name, val);
            return val;
        }
        lox_type operator()(logical_expression* exr)
        {
            auto left = std::visit(*this, to_variant(exr->left.get()));
            if(exr->tk.type == token_type::Or)
            {
                if(is_truthy(left)) return true;
            }
            else
            {
                if(!is_truthy(left)) return false;
            }
            return is_truthy(std::visit(*this, to_variant(exr->right.get())));
        }
        void operator()(print_statement* stat)
        {
            std::cout << to_string(std::visit(*this, to_variant(stat->expr.get()))) << '\n';
        }
        void operator()(expression_statement* stat)
        {
            std::visit(*this, to_variant(stat->expr.get()));
        }
        void operator()(variable_statement* stat)
        {
            lox_type val = std::monostate{};
            if(stat->init)
            {
                val = std::visit(*this, to_variant(stat->init.get()));
            }
            env.define(stat->name.text, std::move(val));
        }
        void operator()(block_statement* stat)
        {
            env.push_scope();
            for(auto& stat : stat->statements)
            {
                std::visit(*this, to_variant(stat.get()));
            }
            env.pop_scope();
        }
        void operator()(if_statement* stat)
        {
            auto cond = std::visit(*this, to_variant(stat->condition.get()));
            if(is_truthy(cond))
            {
                std::visit(*this, to_variant(stat->then_statement.get()));
            }
            else if(stat->else_statement)
            {
                std::visit(*this, to_variant(stat->else_statement.get()));
            }
        }
    private:
        static std::string to_string(const lox_type& type)
        {
            if(std::holds_alternative<std::monostate>(type)) return "nil";
            if(std::holds_alternative<std::string>(type)) return std::get<std::string>(type);
            if(std::holds_alternative<double>(type)) return std::to_string(std::get<double>(type));
            return "unknown";
        }
        static void check_number_operand(int line, const lox_type& t)
        {
            if(!std::holds_alternative<double>(t)) throw execution_error("Expected a number in operation", line);
        }
        static void check_number_operands(int line, const lox_type& t, const lox_type& t2)
        {
            if(!std::holds_alternative<double>(t) || !std::holds_alternative<double>(t2))
                throw execution_error("Expected a number in operation", line);
        }
        static bool is_truthy(const lox_type& t)
        {
            if(std::holds_alternative<std::monostate>(t)) return false;
            if(std::holds_alternative<bool>(t)) return std::get<bool>(t);
            return true;
        }
        environment env;
    };
}
