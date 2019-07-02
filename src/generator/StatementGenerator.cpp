#ifndef STATEMENT_GEN
#define STATEMENT_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "ExpressionGenerator.cpp"
#include "AskAgentsetGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../parser/AST.hpp"

namespace generator {
    std::string getString(const ast::statement& e);

    struct statement_visitor : boost::static_visitor<std::string>
    {
        std::string operator()(const ast::variable_declaration& e) const;
        std::string operator()(const ast::assignment& e) const;
        std::string operator()(const ast::single_word_statement& e) const;
        std::string operator()(const ast::setxy_statement& e) const;
        std::string operator()(const ast::function_call& e) const;
        std::string operator()(const ast::ask_agentset& e) const;
        std::string operator()(const ast::ask_agent& e) const;
        std::string operator()(const ast::create_agentset& e) const;
        std::string operator()(const ast::move_statement& e) const;
        std::string operator()(const ast::hatch_statement& e) const;
        std::string operator()(const ast::if_statement& e) const;
        std::string operator()(const ast::while_statement& e) const;
        std::string operator()(const ast::statement_list& e) const;
    };

    std::string statement_visitor::operator()(const ast::variable_declaration& e) const {
        std::string var_name = removeInvalidChars(e.name.name);
        processor::Types type = processor::variable_types[var_name];

        std::string result = "";
        switch(type) {
            case processor::string_type: result += "std::string " + var_name; break;
            case processor::double_type: result += "double " + var_name; break;
            case processor::bool_type: result += "bool " + var_name; break;
            default: result += "auto " + var_name; break;
        }

        if (e.value) {
            result += " = " + getString(*e.value);
        }
        
        return result + ";\n";
    }

    std::string statement_visitor::operator()(const ast::assignment& e) const {
        std::string v_name = e.name.name;
        auto it = std::find(agentAttributes.begin(), agentAttributes.end(), v_name);

        std::string ctx = "";
        if (it != agentAttributes.end()) ctx += context;
        return ctx + removeInvalidChars(v_name) + " = " + getString(e.value) + ";\n";
    }

    std::string statement_visitor::operator()(const ast::single_word_statement& e) const {
        switch(e.keyword) {
            case ast::die: return "if(_exists) " + context + "remove();\n";
            case ast::clear_all: return ""; // Doesn't exist
            case ast::tick: return ""; // Doesn't exist
            case ast::reset_ticks: return ""; // Doesn't exist
        }
        return "";
    }

    std::string statement_visitor::operator()(const ast::setxy_statement& e) const {
        return context + "setxy(" + getString(e.x) + ", " + getString(e.y) + ");\n";
    }

    std::string statement_visitor::operator()(const ast::function_call& e) const {
        return getString(e) + ";\n";
    }

    std::string statement_visitor::operator()(const ast::ask_agentset& e) const {
        return getString(e);
    }

    std::string statement_visitor::operator()(const ast::ask_agent& e) const {
return "";
    }

    std::string statement_visitor::operator()(const ast::create_agentset& e) const {
return "";
    }

    std::string statement_visitor::operator()(const ast::move_statement& e) const {
        int dir = static_cast<int>(e.direction);
        return context + "mv(" + std::to_string(e.direction) + ", " + getString(e.quantity) + ");\n";
    }

    std::string statement_visitor::operator()(const ast::hatch_statement& e) const {
        std::string ret = "for (int i = 0; i < " + getString(e.quantity) + "; ++i) {\n";
        ret += "Turtle *child = new Turtle(_id + \"_\" + std::to_string(i));\n";
        ret += "_world->addAgent(child);\n";
        context = "child->";
        ret += getString(e.body);
        context = "";
        ret += "}\n";
        return ret;
    }

    std::string statement_visitor::operator()(const ast::if_statement& e) const {
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

    std::string statement_visitor::operator()(const ast::while_statement& e) const {
        std::string ret = "while (" + getString(e.condition) + ") {\n";
        for (auto it = e.body.begin(); it != e.body.end(); ++it) {
            ret += getString(*it);
        }
        ret += "}\n";
        return ret;
    }

    std::string statement_visitor::operator()(const ast::statement_list& e) const {
        std::string ret = "";
        for (auto it = e.begin(); it != e.end(); ++it) {
            ret += getString(*it);
        }
        return ret;
    }

    std::string getString(const ast::statement& e) {
        return boost::apply_visitor(statement_visitor(), e);
    }

    std::string getString(const ast::statement_list& e) {
        std::string ret = "";
        
        for (auto it = e.begin(); it != e.end(); ++it) {
            ret += getString(*it);
        }
        
        return ret;
    }
}

#endif