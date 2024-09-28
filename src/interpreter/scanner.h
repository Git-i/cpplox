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
    public:
        explicit scan_error(const std::string_view msg): message(msg){}
        [[nodiscard]] const char* what() const noexcept override
        {
            return message.c_str();
        }
    };
    class scanner {
        std::istream& source;
    public:
        scanner(std::istream& src) : source(src) {};
        std::vector<token> scan() noexcept(false);
    private:
        char advance();
        [[nodiscard]] char peek();
        bool next_is(char expected);
        void scan_token(std::vector<token>& tokens);
        void add_token(token_type t, std::vector<token>& tokens);
        size_t current_pos = 0;
    };
}
