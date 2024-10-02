#pragma once
#include <span>

#include "expression.h"
namespace cpplox
{
    class ast_printer {
    public:
        static std::string parenthesize(std::string_view name, std::span<expression* const> ts)
        {
            std::string ret = "(";
            ret += name;
            for(auto expr: ts)
            {
                ret += " ";
                ret += std::visit(ast_printer{}, to_variant(expr));
            }
            ret += ")";
            return ret;
        }
        std::string operator()(ternary_expression* exr) const
        {
            return parenthesize("?", {{exr->condition.get(), exr->valid.get(), exr->invalid.get()}});
        }
        std::string operator()(binary_expression* exr) const
        {
            return parenthesize(exr->operator_token.text, {{exr->left.get(), exr->right.get()}});
        };
        std::string operator()(unary_expression* exr) const
        {
            return parenthesize(exr->operator_token.text, {{exr->operand.get()}});
        }
        std::string operator()(literal_expression* exr) const
        {
            class literal_visitor
            {
            public:
                std::string operator()(std::string& str) const { return str; }
                std::string operator()(double& dbl) const { return std::to_string(dbl); }
                std::string operator()(std::monostate&) const { return "nil"; }
                std::string operator()(bool& b) const { return b ?"true" : "false"; }
                std::string operator()(void*& b) const { return "Object"; }
            };
            return std::visit(literal_visitor{}, exr->value);
        }
        std::string operator()(variable_expression* exr) const
        {
            return "var";
        }
        std::string operator()(group_expression* exr) const
        {
            return parenthesize("group", {{exr->operand.get()}});
        }

    };

}
