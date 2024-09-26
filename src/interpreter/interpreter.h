#pragma once
#include <istream>
namespace cpplox {

    class interpreter {
        std::istream& input;
        std::ostream& errors;
    public:
        interpreter(std::istream& in, std::ostream& out) : input(in), errors(out) {};
        void run();
    private:
        void run_line(const std::string& line, int line_no);
        void report_error(int line, const std::string& message);
    };

}
