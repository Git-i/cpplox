#pragma once
#include "expression.h"
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
        lox_type operator()(ternary_expression* exr) const
        {
            auto cond = std::visit(*this, to_variant(exr->condition.get()));
            if(is_truthy(cond))
            {
                return std::visit(*this, to_variant(exr->valid.get()));
            }
            return std::visit(*this, to_variant(exr->invalid.get()));
        }
        lox_type operator()(binary_expression* exr) const
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
        lox_type operator()(unary_expression* exr) const
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
        lox_type operator()(group_expression* exr) const
        {
            return std::visit(*this, to_variant(exr->operand.get()));
        }
    private:
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

    };
}