#ifndef TYPE_INFERENCE_CPP
#define TYPE_INFERENCE_CPP

#include "AST.hpp"

namespace Inference {
    
    enum Types {
        string_type,
        double_type,
        bool_type
    };

    struct Inferer {
        Inferer () {}

        // Recursive cases
        Types infer (ast::expression expr);
        Types infer (ast::operation expr);
        Types infer (ast::variable expr);
        Types infer (ast::unary expr);
        Types infer (ast::function_call expr);
        Types infer (ast::random_statement expr);
        Types infer (ast::count_agentset expr);

        // Base cases
        Types infer (std::string expr);
        Types infer (double expr);
        Types infer (bool expr);
    };
}

#endif