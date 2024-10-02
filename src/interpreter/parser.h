#pragma once
#include <optional>
#include <queue>
#include <span>

#include "expression.h"
#include "scanner.h"
#include "statement.h"

namespace cpplox {
    class parse_error : public std::exception
    {
        int line;
        std::string detail;
    public:
        [[nodiscard]] const char* what() const noexcept override
        {
            return detail.c_str();
        }
        parse_error(std::string_view what, int line) : line(line), detail(what) {}
        [[nodiscard]] int line_no() const {return line; }
    };
    class parser {
        scanner& m_scanner;
    public:
        explicit parser(scanner& scn): m_scanner(scn) {}
        std::unique_ptr<statement> parse();
    private:
        token get();
        token peek();
        void synchronize();
        std::optional<token> next_is(std::span<const token_type>);

        std::unique_ptr<statement> declaration();
        std::unique_ptr<statement> variable_declaration();
        std::unique_ptr<statement> stat();
        std::unique_ptr<statement> print_stat();
        std::unique_ptr<statement> expr_stat();


        std::unique_ptr<expression> expr();
        std::unique_ptr<expression> assignment();
        std::unique_ptr<expression> ternary();
        std::unique_ptr<expression> equality();
        std::unique_ptr<expression> comparison();
        std::unique_ptr<expression> term();
        std::unique_ptr<expression> factor();
        std::unique_ptr<expression> unary();
        std::unique_ptr<expression> primary();
        std::queue<token> peek_buffer;
    };
} // cpplox
