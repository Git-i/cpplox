#include "interpreter.h"
#include "scanner.h"
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
        scanner sc(str);
        try
        {
            auto tokens = sc.scan();
        }
        catch(const scan_error& e)
        {
            report_error(line_no, e.what());
        }

    }
    void interpreter::report_error(int line, const std::string& message)
    {
        errors << "[line " <<   line << ": " << message << std::endl;
    }


}