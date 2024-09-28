#pragma once
#include <memory>
#include <variant>

#include "token.h"

namespace cpplox {
    struct binary_expression;
    struct unary_expression;
    struct literal_expression;
    struct group_expression;
    template<typename T> concept binary_expression_visitor = requires(const T a, binary_expression* b)
    {
        { a.visit_binary_expression(b) };
    };
    template<typename T> concept unary_expression_visitor = requires(const T a, unary_expression* b)
    {
        { a.visit_unary_expression(b) };
    };
    template<typename T> concept literal_expression_visitor = requires(const T a, literal_expression* b)
    {
        { a.visit_literal_expression(b) };
    };
    template<typename T> concept group_expression_visitor = requires(const T a, group_expression* b)
    {
        { a.visit_group_expression(b) };
    };
    class expression
    {
    public:
        virtual ~expression() = default;
    };
    struct binary_expression : public expression {
        std::unique_ptr<expression> left;
        std::unique_ptr<expression> right;
        token operator_token;
        template<binary_expression_visitor V>  auto accept_visitor(const V& v) {return v.visit_binary_expression(this);}
    };
    struct unary_expression : public expression
    {
        std::unique_ptr<expression> operand;
        token operator_token;
        template<unary_expression_visitor V>  auto accept_visitor(const V& v) {return v.visit_unary_expression(this);}
    };
    struct group_expression : public expression
    {
        std::unique_ptr<expression> operand;
        template<group_expression_visitor V>  auto accept_visitor(const V& v) {return v.visit_group_expression(this);}
    };
    struct literal_expression : public expression
    {
        std::variant<std::string, double> value;
        template<literal_expression_visitor V>  auto accept_visitor(const V& v) {return v.visit_literal_expression(this);}
    };


} // cpplox
