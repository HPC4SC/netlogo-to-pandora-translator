#include "TypeInference.hpp"
#include "AST.hpp"

namespace Inference {

    // Base cases
    enum Types Inferer::infer (std::string expr) {
        return string_type;
    }

    enum Types Inferer::infer (double expr) {
        return double_type;
    }

    enum Types Inferer::infer (bool expr) {
        return bool_type;
    }

    // Recursive cases
    enum Types Inferer::infer (ast::count_agentset expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::random_statement expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::function_call expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::unary expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::variable expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::operation expr) {
return bool_type;
    }

    enum Types Inferer::infer (ast::expression expr) {
        enum Types first_type = this->infer(expr.first);

        // If there is only one node
        if (expr.rest.empty())
            return first_type;

        // Then, there is an operator
        enum Types second_type;
        for (auto it = expr.rest.begin(); it != expr.rest.end(); ++it) {
            switch (it->operator_) {
                // Boolean expressions (always return boolean)
                case ast::op_not:
                case ast::op_equal:
                case ast::op_not_equal:
                case ast::op_less:
                case ast::op_less_equal:
                case ast::op_greater:
                case ast::op_greater_equal:
                case ast::op_and:
                case ast::op_or: return bool_type; break;

                // Arithmetic expressions (could be numeric or string)
                case ast::op_plus:
                case ast::op_minus:
                case ast::op_times:
                case ast::op_divide: second_type = this->infer(*it); break;
            }
        }
        if (first_type == second_type)
            return first_type;
        else
            throw std::logic_error("TypeInference: Types don't match.");
    }
}