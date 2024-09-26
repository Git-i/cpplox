#include "interpreter.h"

namespace cpplox {
    void interpreter::run()
    {
        std::string str;
        int line = 0;
        while(std::getline(input, str))
        {
            run_line(str, ++line);
        }
    }
    void interpreter::run_line(const std::string& str, int line_no)
    {

    }
    void interpreter::report_error(int line, const std::string& message)
    {
        errors << "[line " <<   line << ": " << message << std::endl;
    }


}