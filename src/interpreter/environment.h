#pragma once
#include <unordered_map>
#include <string>
#include "expression.h"
namespace cpplox{
    class env_error : std::exception{
        std::string message;
    public:
        [[nodiscard]] const char* what() const noexcept override{
            return message.c_str();
        }
        env_error(std::string msg) : message(std::move(msg)){}
    };
    struct environment{
        lox_type get(const token& name)const{
            if(variables.contains(name.text)) return variables.at(name.text);
            throw env_error("Undefined variable " + name.text);
        }
        void define(const std::string& name, lox_type value)
        {
            variables[name] = std::move(value);
        }
        std::unordered_map<std::string, lox_type> variables;
    };
}