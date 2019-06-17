#ifndef EXPRESSION_GEN
#define EXPRESSION_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>
#include <algorithm>

#include "../parser/AST.hpp"

namespace generator {
    std::string getString(const ast::expression& e);
    std::string getString(const ast::operation& op);
    std::string getString(const ast::unary& op);
    std::string getString(const ast::optoken& opt);
    std::string getString(const ast::function_call& op);
    std::string getString(const ast::random_statement& op);
    std::string getString(const ast::variable& op);

    std::string removeInvalidChars (std::string name) {
        name.erase(std::remove(name.begin(), name.end(), '-'), name.end());
        name.erase(std::remove(name.begin(), name.end(), '?'), name.end());
        name.erase(std::remove(name.begin(), name.end(), '%'), name.end());
        return name;
    }

    struct expression_visitor : boost::static_visitor<std::string>
    {
        std::string operator()(const double& v) const { return boost::lexical_cast<std::string>(v); }
        std::string operator()(const bool& b) const { return b ? "true" : "false"; }
        std::string operator()(const std::string& v) const { return  "\"" + v + "\""; }
        std::string operator()(const ast::unary& e) const { return getString(e); }
        std::string operator()(const ast::expression& e) const { return getString(e); }
        std::string operator()(const ast::function_call& e) const { return context + getString(e); }
        std::string operator()(const ast::random_statement& e) const { return getString(e); }
        std::string operator()(const ast::variable& e) const { 
            auto it = std::find(agentAttributes.begin(), agentAttributes.end(), e.name);
            std::string ctx = "";
            if (it != agentAttributes.end()) ctx = context;
            return ctx + removeInvalidChars(e.name); 
        }
    };

    std::string getString(const ast::expression& e) {
        std::string result;
        std::string s = boost::apply_visitor(expression_visitor(), e.first);

        if (e.rest.size() > 0) {
            result = "( " + s;
            for (auto it = e.rest.begin(); it != e.rest.end(); ++it) {
                result += getString(*it);
            }
            result += " )";
        }
        else {
            result = s;
        }

        return result;
    }

    std::string getString(const ast::variable& op) {
        std::cout << op.name << std::endl;
        auto it = std::find(agentAttributes.begin(), agentAttributes.end(), op.name);

        return (it != agentAttributes.end()) ? context + op.name : op.name;
    }
    
    std::string getString(const ast::function_call& op) {
        auto it = op.args.begin();
        std::string args = "";
        if (it != op.args.end()) {
            args = getString(*it);
            ++it;
            for (it; it != op.args.end(); ++it) {
                args += ", " + getString(*it);
            }
        }
        return context + removeInvalidChars(op.function_name) + "(" + args + ")";
    }

    std::string getString(const ast::random_statement& op) {
        return "random(" + getString(op.value) + ")";
    }

    std::string getString(const ast::unary& op) {
        std::string s = boost::apply_visitor(expression_visitor(), op.operand_);
        return getString(op.operator_) + s;
    }

    std::string getString(const ast::operation& op) {
        std::string s = boost::apply_visitor(expression_visitor(), op.operand_);
        return " " + getString(op.operator_) + " " + s;
    }

    std::string getString(const ast::optoken& opt) {
        switch(opt) {
            case ast::op_plus: return "+";
            case ast::op_minus: return "-";
            case ast::op_times: return "*";
            case ast::op_divide: return "/";
            case ast::op_not: return "!";
            case ast::op_equal: return "==";
            case ast::op_not_equal: return "!=";
            case ast::op_less: return "<";
            case ast::op_less_equal: return "<=";
            case ast::op_greater: return ">";
            case ast::op_greater_equal: return ">=";
            case ast::op_and: return "&&";
            case ast::op_or: return "||";
        }
        return "undf";
    }
}

#endif