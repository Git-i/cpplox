#pragma once
#include <memory>
#include <variant>

#include "token.h"

namespace cpplox {
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
        std::variant<std::monostate, std::string, double> value;
    };
    using expressions = std::variant<binary_expression*, unary_expression*, group_expression*, literal_expression*>;
    expressions to_variant(expression& expr)
    {
        if(auto p = dynamic_cast<binary_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<unary_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<literal_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<group_expression*>(&expr)) return p;
        else return {};
    };

} // cpplox
