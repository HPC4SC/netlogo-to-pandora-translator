#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>

#include "AST.hpp"


namespace testing {
    std::string getString(ast::expression& e);
    std::string getString(ast::operation& op);
    std::string getString(ast::unary& op);
    std::string getString(ast::optoken& opt);
    std::string getString(ast::function_call& op);

    struct visitor : boost::static_visitor<std::string>
    {
        std::string operator()(std::string& v) const { return v; }
        std::string operator()(double& v) const { return boost::lexical_cast<std::string>(v); }
        std::string operator()(bool& b) const { return b ? "True" : "False"; }

        std::string operator()(std::string& e) const { return e; }
        std::string operator()(ast::function_call& e) const { return getString(e); }
        std::string operator()(ast::expression& e) const { return getString(e); }
        std::string operator()(ast::unary& e) const { return getString(e); }
    };

    std::string getString(ast::expression& e) {
        std::string result;
        std::string s = boost::apply_visitor(visitor(), e.first);

        if (e.rest.begin() != e.rest.end()) {
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
    
    std::string getString(ast::function_call& op) {
        auto it = op.args.begin();
        std::string args = getString(*it);
        ++it;
        for (it; it != op.args.end(); ++it) {
            args += ", " + getString(*it);
        }
        return getString(op.function_name) + "(" + args + ")";
    }

    std::string getString(ast::unary& op) {
        std::string s = boost::apply_visitor(visitor(), op.operand_);
        return getString(op.operator_) + s;
    }

    std::string getString(ast::operation& op) {
        std::string s = boost::apply_visitor(visitor(), op.operand_);
        return " " + getString(op.operator_) + " " + s;
    }

    std::string getString(ast::optoken& opt) {
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