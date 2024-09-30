//
// Created by eric on 9/29/24.
//

#pragma once
#include <optional>
#include <queue>
#include <span>

#include "expression.h"
#include "scanner.h"

namespace cpplox {

    class parser {
        scanner& m_scanner;
    public:
        explicit parser(scanner& sc): m_scanner(sc) {}
        std::unique_ptr<expression> parse_expression();
    private:
        token get();
        token peek();
        std::optional<token> next_is(std::span<const token_type>);
        std::unique_ptr<expression> equality();
        std::unique_ptr<expression> comparison();
        std::unique_ptr<expression> term();
        std::unique_ptr<expression> factor();
        std::unique_ptr<expression> unary();
        std::unique_ptr<expression> primary();
        std::queue<token> peek_buffer;
    };

} // cpplox
