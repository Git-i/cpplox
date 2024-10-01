#include "interpreter.h"
#include "scanner.h"
#include "executor.h"
namespace cpplox {
    void interpreter::run()
    {
        std::string str;
        char c;
        input >> c;
        int line = 0;
        while(std::getline(input, str))
        {
            run_line(str, ++line);
        }
    }
    void interpreter::run_line(const std::string& str, int line_no)
    {
        scanner sc(input);
    }


}