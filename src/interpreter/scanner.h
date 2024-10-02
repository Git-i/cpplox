#pragma once
#include <stdexcept>
#include <string_view>
#include <vector>
#include "token.h"
#include <istream>
#include <cstdint>
#include <unordered_map>

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
        [[nodiscard]] bool is_eof();
    private:
        char advance();
        [[nodiscard]] char peek();
        bool next_is(char expected);
        token scan_string();
        token scan_number(char start = 0);
        token scan_identifier(char start = 0);
        char peek_next();
        token scan_token();
        int line = 1;
        size_t current_pos = 0;
        static std::unordered_map<std::string, token_type> keyword_map;
    };
}
