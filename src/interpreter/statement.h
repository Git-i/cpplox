#pragma once
#include <memory>
#include "expression.h"
namespace cpplox
{
    class statement
    {
    public:
        virtual ~statement() = default;
    };
    class expression_statement : public statement
    {
    public:
        std::unique_ptr<expression> expr;
    };
    class print_statement : public statement
    {
    public:
        std::unique_ptr<expression> expr;
    };
    class variable_statement : public statement
    {
    public:
        token name;
        std::unique_ptr<expression> init;
    };
    using statements = std::variant<expression_statement*, print_statement*, variable_statement*>;
    statements to_variant(statement* stmt);


}
