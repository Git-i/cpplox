#pragma once
#include <string_view>
namespace cpplox {
    enum class token_type {
        LParen, RParen, RBrace, LBrace, Comma, Period, Minus,
        Plus, Semicolon, Slash, Star,

        Equal, DoubleEqual, Greater, GreaterEqual, Less, LessEqual,
        Exclamation, ExclamationEqual,

        Identifier, StringLiteral, NumericLiteral,

        And, Or, Nil, Class, Else, False, Fun, For, If, Print, Return,
        Super, This, True, Var, While

    };
    struct token {
        token_type type;
        std::string text;
        int line = 0;
    };
}