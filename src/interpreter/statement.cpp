#include "statement.h"
namespace cpplox {
    statements to_variant(statement* stmt)
    {
        if(auto p = dynamic_cast<print_statement*>(stmt)) return p;
        if(auto p = dynamic_cast<expression_statement*>(stmt)) return p;
        if(auto p = dynamic_cast<variable_statement*>(stmt)) return p;
    }
}