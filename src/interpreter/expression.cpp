
#include "expression.h"

namespace cpplox {
    expressions to_variant(expression* expr)
    {
        if(auto p = dynamic_cast<binary_expression*>(expr)) return p;
        if(auto p = dynamic_cast<unary_expression*>(expr)) return p;
        if(auto p = dynamic_cast<literal_expression*>(expr)) return p;
        if(auto p = dynamic_cast<group_expression*>(expr)) return p;
        if(auto p = dynamic_cast<ternary_expression*>(expr)) return p;
        if(auto p = dynamic_cast<variable_expression*>(expr)) return p;
        if(auto p = dynamic_cast<assignment_expression*>(expr)) return p;
        return {};
    };

}