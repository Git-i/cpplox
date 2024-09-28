#include <filesystem>
#include <fstream>
#include <iostream>
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
    cpplox::scanner sc(std::cin);
    while(true)
    {
        auto tk = sc.scan();
        std::cout << static_cast<int>(tk.type) << " " << tk.text << std::endl;
    }
}
