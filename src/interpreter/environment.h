#pragma once
#include <unordered_map>
#include <string>
#include <vector>

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
    struct environment_frame{
        lox_type get(const token& name) const {
            if(variables.contains(name.text)) return variables.at(name.text);
            throw env_error("Undefined variable " + name.text);
        }
        void define(const std::string& name, lox_type value)
        {
            variables[name] = std::move(value);
        }
        void assign(const token& name, lox_type value)
        {
            if(variables.contains(name.text)) variables.at(name.text) = std::move(value);
            else throw env_error("Undefined variable " + name.text);
        }
        bool contains(const token& name) const
        {
            return variables.contains(name.text);
        }
        std::unordered_map<std::string, lox_type> variables;
    };
    class environment
    {
        std::vector<environment_frame> environment_stack;
    public:
        environment() {environment_stack.emplace_back();}
        void push_scope() { environment_stack.emplace_back(); }
        void pop_scope() { environment_stack.pop_back(); }
        [[nodiscard]] lox_type get(const token& name) const
        {
            for(auto i = environment_stack.size(); i > 0; i--)
            {
                if(const auto idx = i - 1; environment_stack[idx].contains(name))
                    return environment_stack[idx].get(name);
            }
            throw env_error("Undefined variable " + name.text);
        }
        void assign(const token& name, lox_type value)
        {
            for(auto i = environment_stack.size(); i > 0; i--)
            {
                if(const auto idx = i - 1; environment_stack[idx].contains(name))
                {
                    environment_stack[idx].assign(name, std::move(value));
                    return;
                }
            }
            throw env_error("Undefined variable " + name.text);
        }
        void define(const std::string& name, lox_type value)
        {
            environment_stack.back().define(name, std::move(value));
        }
    };
}