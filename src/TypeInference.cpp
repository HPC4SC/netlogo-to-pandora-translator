#ifndef TYPE_INFERENCE_CPP
#define TYPE_INFERENCE_CPP

#include "AST.hpp"

namespace inference {

    enum Types {
        string_type,
        double_type,
        bool_type,
        undefined_type,
        void_type
    };

    std::map<std::string, enum Types> function_types;
    std::map<std::string, enum Types> variable_types;

    struct Inferer : boost::static_visitor<enum Types> {
        Inferer () {}

        Types operator()(double& expr) const { return double_type; }
        Types operator()(bool& expr) const { return bool_type; }
        Types operator()(std::string& expr) const { return string_type; }
        Types operator()(ast::unary& expr) const { return bool_type; }
        Types operator()(ast::random_statement& expr) const { return double_type; }
        Types operator()(ast::count_agentset& expr) const { return double_type; }

        Types operator()(ast::variable& expr) const { 
            auto it = variable_types.find(expr.name);
            if (it == variable_types.end())
                return undefined_type;

            return it->second;
        }
        Types operator()(ast::function_call& expr) const { 
            auto it = function_types.find(expr.function_name);
            if (it == function_types.end())
                return undefined_type;

            return it->second;
        }
        Types operator()(ast::expression& expr) const {
            Types first_type = boost::apply_visitor(*this, expr.first);
            // If there is only one node
            if (expr.rest.empty())
                return first_type;

            // Then, there is an operator
            auto it = expr.rest.begin();
            switch (it->operator_) {
                // Arithmetic expressions (could be numeric or string)
                case ast::op_plus:
                case ast::op_minus:
                case ast::op_times:
                case ast::op_divide: return double_type;

                // Boolean expressions (always return boolean)
                default: return bool_type;
            }
        }

        Types operator()(ast::variable_declaration& expr) const {
            std::string name = expr.name.name;

            if (expr.value) {
                Types type = boost::apply_visitor(*this, expr.value);
                variable_types[name] = type;
                return type;
            }

            variable_types[name] = undefined_type;
            return undefined_type; 
        }
        Types operator()(ast::assignment& expr) const {
            std::string name = expr.name.name;
            Types type = boost::apply_visitor(*this, expr.value);
            variable_types[name] = type;
            return type;
        }
        Types operator()(ast::single_word_statement& expr) const { return void_type; }
        Types operator()(ast::setxy_statement& expr) const { return void_type; }
        Types operator()(ast::ask_agentset& expr) const { return void_type; }
        Types operator()(ast::ask_agent& expr) const { return void_type; }
        Types operator()(ast::create_agentset& expr) const { return void_type; }
        Types operator()(ast::move_statement& expr) const { return void_type; }
        Types operator()(ast::if_statement& expr) const {
            boost::apply_visitor(*this, expr.then);
            if (expr.else_) boost::apply_visitor(*this, expr.else_);

            return void_type; 
        }
        Types operator()(ast::while_statement& expr) const {
            boost::apply_visitor(*this, expr.body);

            return void_type; 
        }
        Types operator()(ast::statement_list& expr) const {
            BOOST_FOREACH(ast::statement const& st, expr) boost::apply_visitor(*this, st);
            return void_type; 
        }
    };
}

#endif