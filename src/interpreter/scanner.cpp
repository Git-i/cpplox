#include "scanner.h"

namespace cpplox
{
    std::vector<token> scanner::scan()
    {
        std::vector<token> tokens;
        scan_token(tokens);
        return tokens;
    }

    void scanner::scan_token(std::vector<token>& tokens)
    {
        switch (advance())
        {
        case '(': add_token(token_type::LParen, tokens);
            break;
        case ')': add_token(token_type::RParen, tokens);
            break;
        case '{': add_token(token_type::LBrace, tokens);
            break;
        case '}': add_token(token_type::RBrace, tokens);
            break;
        case ',': add_token(token_type::Comma, tokens);
            break;
        case '.': add_token(token_type::Period, tokens);
            break;
        case '-': add_token(token_type::Minus, tokens);
            break;
        case '+': add_token(token_type::Plus, tokens);
            break;
        case ';': add_token(token_type::Comma, tokens);
            break;
        case '*': add_token(token_type::Star, tokens);
            break;
        case '!':
            add_token(next_is('=') ? token_type::ExclamationEqual : token_type::Exclamation, tokens);
            break;
        case '=':
            add_token(next_is('=') ? token_type::DoubleEqual : token_type::Equal, tokens);
            break;
        case '<':
            add_token(next_is('=') ? token_type::LessEqaul : token_type::Less, tokens);
            break;
        case '>':
            add_token(next_is('>') ? token_type::GreaterEqual : token_type::Greater, tokens);
            break;
        case '/':
            if(next_is('/')) while (peek() != '\n') advance();
            else add_token(token_type::Slash, tokens);
            break;
        case ' ': [[fallthrough]];
        case '\r': [[fallthrough]];
        case '\t': [[fallthrough]];
            break;
        default: throw scan_error("");
        }
    }

    char scanner::peek()
    {
        return static_cast<char>(source.peek());
    }

    bool scanner::next_is(const char expected)
    {
        if (expected != peek()) return false;
        advance();
        return true;
    }

    void scanner::add_token(token_type t, std::vector<token>& tokens)
    {
        tokens.push_back(token{.type = t, .text = {source.data() + current_pos, 1}, .line = 0});
    }

    char scanner::advance()
    {
        return static_cast<char>(source.get());
    }
}
