#ifndef ASK_AGENTSET_GEN
#define ASK_AGENTSET_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>
#include <algorithm>

#include "../parser/AST.hpp"
#include "ExpressionGenerator.cpp"
#include "StatementGenerator.cpp"

namespace generator {
    std::string getString(const ast::statement_list& e);

    struct data {
        ast::agent_type type;

        bool other = false;
        bool here = false;
        bool b_n_of = false;
        bool b_with = false;
        
        ast::expression n_of;
        ast::expression with;
    } expr_data;

    struct agentset_expression_visitor : boost::static_visitor<void>
    {
        void operator()(const ast::base_agentset& e) const { 
            expr_data.type = e.type; 
        }
        void operator()(const ast::agentset_here& e) const { 
            expr_data.type = e.type; 
            expr_data.here = true; 
        }
        void operator()(const ast::n_of_agentset& e) const { 
            expr_data.b_n_of = true; 
            expr_data.n_of = e.expr;
            boost::apply_visitor(agentset_expression_visitor(), e.agentset_expr);
        }
        void operator()(const ast::agentset_with_reporter& e) const { 
            expr_data.b_with = true;
            expr_data.with = e.expr;
            boost::apply_visitor(agentset_expression_visitor(), e.agentset_expr);
        }
        void operator()(const ast::other_agentset& e) const {
            expr_data.other = true;
            boost::apply_visitor(agentset_expression_visitor(), e.agentset_expr);
        }
    };

    std::string generateTurtleExpr (const ast::ask_agentset& expr) {
        std::string ret = "";

        if (expr_data.here) {
            ret += "Engine::AgentsVector neighbours = getWorld()->getNeighbours(this,1);\n";
            ret += "if (neighbours.size() > 0) {\n";
            ret += "random_shuffle(neighbours.begin(),neighbours.end());\n";
            ret += "int count = 0;\n";
		    ret += "Engine::AgentsVector::iterator neighbour = neighbours.begin();\n";
            ret += "while (neighbour != neighbours.end()) {\n";

            if (expr_data.b_n_of) {
                ret +=" if (count >=" + getString(expr_data.n_of) + ") break;\n";
            }

            ret += "Turtle* turtle = dynamic_cast<Turtle*>(neighbour->get());\n";
            context = "turtle->";

            if (expr_data.b_with) {
                ret += "if (" + getString(expr_data.with) + ") {\n";
            }

            ret += getString(expr.body);

            if (expr_data.b_with) {
                ret += "}\n";
            }

            context = "";

            ret += "++count;\n";
            ret += "}\n";
            ret += "}\n";
        }

        return ret;
    }

    std::string getString (const ast::ask_agentset& ask) {
        boost::apply_visitor(agentset_expression_visitor(), ask.type);

        switch (expr_data.type) {
            case ast::turtle: return generateTurtleExpr(ask); break;
        }

        return "";
    }
}

#endif