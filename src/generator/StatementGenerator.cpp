#ifndef STATEMENT_GEN
#define STATEMENT_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "ExpressionGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../parser/AST.hpp"

namespace generator {
    std::string getString(ast::statement& e);

    struct statement_visitor : boost::static_visitor<std::string>
    {
        std::string operator()(ast::variable_declaration& e) const;
        std::string operator()(ast::assignment& e) const;
        std::string operator()(ast::single_word_statement& e) const;
        std::string operator()(ast::setxy_statement& e) const;
        std::string operator()(ast::function_call& e) const;
        std::string operator()(ast::ask_agentset& e) const;
        std::string operator()(ast::ask_agent& e) const;
        std::string operator()(ast::create_agentset& e) const;
        std::string operator()(ast::move_statement& e) const;
        std::string operator()(ast::if_statement& e) const;
        std::string operator()(ast::while_statement& e) const;
        std::string operator()(ast::statement_list& e) const;
    };

    std::string statement_visitor::operator()(ast::variable_declaration& e) const {
        std::string var_name = removeInvalidChars(e.name.name);
        inference::Types type = inference::variable_types[var_name];

        std::string result = "";
        switch(type) {
            case inference::string_type: result += "std::string " + var_name; break;
            case inference::double_type: result += "double " + var_name; break;
            case inference::bool_type: result += "bool " + var_name; break;
            default: result += "auto " + var_name; break;
        }

        if (e.value) {
            result += " = " + getString(*e.value);
        }
        
        return result + ";\n";
    }

    std::string statement_visitor::operator()(ast::assignment& e) const {  
        return removeInvalidChars(e.name.name) + " = " + getString(e.value) + ";\n";
    }

    std::string statement_visitor::operator()(ast::single_word_statement& e) const {
        switch(e.keyword) {
            case ast::die: return "die();\n";
            case ast::clear_all: return "clear_all();\n";
            case ast::tick: return "tick();\n";
            case ast::reset_ticks: return "reset_ticks();\n";
        }
        return "";
    }

    std::string statement_visitor::operator()(ast::setxy_statement& e) const {
        return "setxy(" + getString(e.x) + ", " + getString(e.y) + ");\n";
    }

    std::string statement_visitor::operator()(ast::function_call& e) const {
        return getString(e) + ";\n";
    }

    std::string statement_visitor::operator()(ast::ask_agentset& e) const {
return "";
    }

    std::string statement_visitor::operator()(ast::ask_agent& e) const {
return "";
    }

    std::string statement_visitor::operator()(ast::create_agentset& e) const {
return "";
    }

    std::string statement_visitor::operator()(ast::move_statement& e) const {
return "";
    }

    std::string statement_visitor::operator()(ast::if_statement& e) const {
        std::string ret = "if (" + getString(e.condition) + ") {\n";
        for (auto it = e.then.begin(); it != e.then.end(); ++it) {
            ret += getString(*it);
        }
        ret += "}\n";
        if (e.else_) {
            ret += "else {\n";
            for (auto it = (*e.else_).begin(); it != (*e.else_).end(); ++it) {
                ret += getString(*it);
            }
            ret += "}\n";
        }
        return ret;
    }

    std::string statement_visitor::operator()(ast::while_statement& e) const {
        std::string ret = "while (" + getString(e.condition) + ") {\n";
        for (auto it = e.body.begin(); it != e.body.end(); ++it) {
            ret += getString(*it);
        }
        ret += "}\n";
        return ret;
    }

    std::string statement_visitor::operator()(ast::statement_list& e) const {
        std::string ret = "";
        for (auto it = e.begin(); it != e.end(); ++it) {
            ret += getString(*it);
        }
        return ret;
    }

    std::string getString(ast::statement& e) {
        return boost::apply_visitor(statement_visitor(), e);
    }

    std::string getString(ast::statement_list& e) {
        std::string ret = "";
        for (auto it = e.begin(); it != e.end(); ++it) {
            ret += getString(*it);
        }
        return ret;
    }
}

#endif