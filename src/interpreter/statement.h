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
    using statements = std::variant<expression_statement*, print_statement*>;
    statements to_variant(statement* stmt);


}
