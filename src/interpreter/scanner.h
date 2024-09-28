#pragma once
#include <stdexcept>
#include <string_view>
#include <vector>
#include "token.h"
#include <istream>
namespace cpplox {
    class scan_error : public std::exception
    {
        std::string message;
        int line;
    public:
        scan_error(const std::string_view msg, int line): message(msg), line(line) {}
        [[nodiscard]] const char* what() const noexcept override
        {
            return message.c_str();
        }
    };
    class scanner {
        std::istream& source;
    public:
        scanner(std::istream& src) : source(src) {};
        token scan() noexcept(false);
    private:
        char advance();
        [[nodiscard]] bool is_eof();
        [[nodiscard]] char peek();
        bool next_is(char expected);
        token scan_string();
        token scan_token();
        int line;
        size_t current_pos = 0;
    };
}
