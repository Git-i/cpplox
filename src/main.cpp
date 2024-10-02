#include <filesystem>
#include <fstream>
#include <iostream>

#include "interpreter/ast_printer.h"
#include "interpreter/executor.h"
#include "interpreter/expression.h"
#include "interpreter/interpreter.h"
#include "interpreter/parser.h"
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
    std::istringstream input(R"("asggae")");
    cpplox::scanner sc(std::cin);
    cpplox::parser p(sc);
    auto tree = p.parse();
    std::visit(cpplox::executor{}, cpplox::to_variant(tree.get()));
}
