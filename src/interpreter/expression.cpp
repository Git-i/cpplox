
#include "expression.h"

namespace cpplox {
    expressions to_variant(expression& expr)
    {
        if(auto p = dynamic_cast<binary_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<unary_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<literal_expression*>(&expr)) return p;
        else if(auto p = dynamic_cast<group_expression*>(&expr)) return p;
        else return {};
    };

}