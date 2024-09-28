#include "scanner.h"

namespace cpplox
{
    token scanner::scan()
    {
        return scan_token();
    }

    token scanner::scan_token()
    {
        while (true)
        switch (advance())
        {
        case '(': return {.type = token_type::LParen};
        case ')': return {.type = token_type::RParen};
        case '{': return {.type = token_type::LBrace};
        case '}': return {.type = token_type::RBrace};
        case ',': return {.type = token_type::Comma};
        case '.': return {.type = token_type::Period};
        case '-': return {.type = token_type::Minus};
        case '+': return {.type = token_type::Plus};
        case ';': return {.type = token_type::Comma};
        case '*': return {.type = token_type::Star};
        case '!':
            return {.type = next_is('=') ? token_type::ExclamationEqual : token_type::Exclamation};
        case '=':
            return {.type = next_is('=') ? token_type::DoubleEqual : token_type::Equal};
            break;
        case '<':
            return {.type = next_is('=') ? token_type::LessEqaul : token_type::Less};
            break;
        case '>':
            return {.type = next_is('>') ? token_type::GreaterEqual : token_type::Greater};
            break;
        case '/':
            if(next_is('/')) while (peek() != '\n' && !is_eof()) advance();
            else return {.type = token_type::Slash};
            break;
        case '"': scan_string(); break;
        case ' ': [[fallthrough]];
        case '\r': [[fallthrough]];
        case '\t': break;
        case '\n': line++; break;
        default: throw scan_error("Invalid character", line);
        }
    }
    token scanner::scan_string()
    {
        std::string str;
        char c = peek();
        while(c != '"' && !is_eof())
        {
            if(c == '\n') line++;
            str.push_back(advance());
            c = peek();
        }
        if(is_eof()) throw scan_error("Unterminated string", line);
        advance();//the last "
        return {.type = token_type::StringLiteral, .text = std::move(str)};
    }

    char scanner::peek()
    {
        return static_cast<char>(source.peek());
    }

    bool scanner::next_is(const char expected)
    {
        if (is_eof() ||expected != peek()) return false;
        advance();
        return true;
    }

    bool scanner::is_eof()
    {
        return source.eof();
    }

    char scanner::advance()
    {
        return static_cast<char>(source.get());
    }
}
