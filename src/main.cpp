#include <filesystem>
#include <fstream>
#include <iostream>

#include "interpreter/ast_printer.h"
#include "interpreter/executor.h"
#include "interpreter/expression.h"
#include "interpreter/interpreter.h"
#include "interpreter/parser.h"
#include "interpreter/scanner.h"

std::string to_string(const cpplox::lox_type& type)
{
    if(std::holds_alternative<std::monostate>(type)) return "nil";
    if(std::holds_alternative<std::string>(type)) return std::get<std::string>(type);
    if(std::holds_alternative<double>(type)) return std::to_string(std::get<double>(type));
}

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
    std::istringstream input(R"("asggae")");
    cpplox::scanner sc(input);
    cpplox::parser p(sc);
    auto tree = p.parse();
    std::cout << to_string(std::visit(cpplox::executor{}, cpplox::to_variant(tree.get()))) << std::endl;
}
