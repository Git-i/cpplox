#include "scanner.h"

namespace cpplox
{
    std::unordered_map<std::string, token_type> scanner::keyword_map{
        {"and", token_type::And},
        {"class", token_type::Class},
        {"else", token_type::Else},
        {"false", token_type::False},
        {"for", token_type::For},
        {"fun", token_type::Fun},
        {"if", token_type::If},
        {"nil", token_type::Nil},
        {"or", token_type::Or},
        {"print", token_type::Print},
        {"return", token_type::Return},
        {"super", token_type::Super},
        {"this", token_type::This},
        {"true", token_type::True},
        {"var", token_type::Var},
        {"while", token_type::While},
    };
    token scanner::scan()
    {
        return scan_token();
    }

    token scanner::scan_token()
    {
        while (true)
        switch (char c = advance())
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
            return {.type = next_is('=') ? token_type::LessEqual : token_type::Less};
            break;
        case '>':
            return {.type = next_is('>') ? token_type::GreaterEqual : token_type::Greater};
            break;
        case '/':
            if(next_is('/')) while (peek() != '\n' && !is_eof()) advance();
            else if(next_is('*'))
            {
                while (peek() != '*' || peek_next() != '/' && !is_eof())
                {
                    if(peek() == '\n') line++;
                    advance();
                }
                if(is_eof()) throw scan_error("unterminated block comment", line);
                advance(); advance();
            }
            else return {.type = token_type::Slash};
            break;
        case '"': return scan_string();
        case ' ': [[fallthrough]];
        case '\r': [[fallthrough]];
        case '\t': break;
        case '\n': line++; break;
        default:
            if(std::isdigit(static_cast<unsigned char>(c))) return scan_number(c);
            else if(std::isalpha(static_cast<unsigned char>(c)) || c == '_') return scan_identifier(c);
            throw scan_error("Invalid character", line);
        }
    }
    char scanner::peek_next()
    {
        source.get();
        char c = source.peek();
        if(!source.unget())
        {
            throw std::runtime_error("istream provided doesn't support unget");
        }
        return c;
    }
    token scanner::scan_identifier(char start)
    {
        std::string str;
        if(start != 0) str.push_back(start);
        while(std::isalnum(peek())) str.push_back(advance());
        auto tt = token_type::Identifier;
        if(keyword_map.contains(str)) tt = keyword_map[str];
        return {.type = tt, .text= str};
    }

    token scanner::scan_number(const char start)
    {
        std::string str;
        if(start != 0) str.push_back(start);
        while(isdigit(peek())) str.push_back(advance());
        if(peek() == '.' && isdigit(peek_next()))
        {
            str.push_back(advance());
            while(isdigit(peek())) str.push_back(advance());
        }
        return {.type = token_type::NumericLiteral, .text = str};
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
