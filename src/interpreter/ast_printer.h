#pragma once
#include "expression.h"
namespace cpplox
{
    class ast_printer {
    public:
        template<typename... T>
        static  void empty(T&&... ts) {};
        template<typename... T>
        static std::string parenthesize(std::string_view name, T&&... ts)
        {
            std::string ret = "(";
            ret += name;
            empty([&ret](auto&& x){
                ret += " ";
                ret += std::visit(ast_printer{}, to_variant(*x));
                return 0;
            }(ts)...);
            ret += ")";
            return ret;
        }
        std::string operator()(binary_expression* exr) const
        {
            return parenthesize(exr->operator_token.text, exr->left.get(), exr->right.get());
        };
        std::string operator()(unary_expression* exr) const
        {
            return parenthesize(exr->operator_token.text, exr->operand.get());
        }
        std::string operator()(literal_expression* exr) const
        {
            class literal_visitor
            {
                std::string operator()(std::string& str) const { return str; }
                std::string operator()(double& dbl) const { return std::to_string(dbl); }
                std::string operator()(std::monostate) const { return "nil"; }
            };
            return std::visit(literal_visitor{}, exr->value);
        }
        std::string operator()(group_expression* exr) const
        {
            return parenthesize("group", exr->operand);
        }

    };

}
