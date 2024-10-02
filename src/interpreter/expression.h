#pragma once
#include <memory>
#include <variant>

#include "token.h"

namespace cpplox {
    using lox_type = std::variant<std::monostate, std::string, double, bool, void*>; //consider std::any?
    class expression
    {
    public:
        virtual ~expression() = default;
    };
    class binary_expression : public expression {
    public:
        std::unique_ptr<expression> left;
        std::unique_ptr<expression> right;
        token operator_token;
    };
    class ternary_expression : public expression
    {
    public:
        std::unique_ptr<expression> valid;
        std::unique_ptr<expression> invalid;
        std::unique_ptr<expression> condition;
    };
    class unary_expression : public expression
    {
    public:
        std::unique_ptr<expression> operand;
        token operator_token;
    };
    class group_expression : public expression
    {
    public:
        std::unique_ptr<expression> operand;
    };
    class literal_expression : public expression
    {
    public:
        lox_type value;
    };
    class variable_expression : public expression
    {
    public:
        token name;
    };
    using expressions = std::variant<binary_expression*, unary_expression*, group_expression*, literal_expression*, ternary_expression*, variable_expression*>;
    expressions to_variant(expression* expr);


} // cpplox
