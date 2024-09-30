#include <filesystem>
#include <fstream>
#include <iostream>

#include "interpreter/ast_printer.h"
#include "interpreter/expression.h"
#include "interpreter/interpreter.h"
#include "interpreter/scanner.h"

int main(int argc, char** argv) {
    /*
    std::istream* input;
    std::ifstream ifs;
    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    else if (argc == 2)
    {
        ifs = std::ifstream(argv[1]);
        input = &ifs;
    }
    else
        input = &std::cin;

    cpplox::interpreter in(*input, std::cerr);
    in.run();
    */
    cpplox::binary_expression expression;
    auto l = std::make_unique<cpplox::unary_expression>();
    auto lit = std::make_unique<cpplox::literal_expression>();
    lit->value = 100.0;
    l->operator_token = cpplox::token{.type = cpplox::token_type::Exclamation, .text = "!"};
    l->operand = std::move(lit);
    auto r = std::make_unique<cpplox::group_expression>();
    auto lit2 = std::make_unique<cpplox::literal_expression>();
    lit2->value = 45.67;
    r->operand = std::move(lit2);
    expression.left = std::move(l);
    expression.right = std::move(r);
    expression.operator_token = cpplox::token{.type = cpplox::token_type::Star, .text = "*"};
    std::cout << std::visit(cpplox::ast_printer{}, cpplox::to_variant(expression)) << std::endl;
}
