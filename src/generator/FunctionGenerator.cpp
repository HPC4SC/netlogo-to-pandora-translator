#ifndef FUNCTION_GEN
#define FUNCTION_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "StatementGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../parser/AST.hpp"

namespace generator {
    std::string getString(processor::Types type) {
        switch(type) {
            case processor::string_type: return "std::string";
            case processor::double_type: return "double";
            case processor::bool_type: return "bool";
            case processor::void_type: return "void";
            default: return "auto";
        }
    }

    std::string getString(ast::return_statement& e) {
        if (e.expr) {
            return "return " + getString(*e.expr) + ";\n";
        }
        return "";
    }

    std::string getString(std::string f_name, ast::function_args& e) {
        std::string res = "(";

        if (e) {
            std::list<std::string> args = *e;
            std::list<processor::Types> arg_types = processor::function_args_types[f_name];

            auto it = args.begin();
            auto it_args = arg_types.begin();

            res += getString(*it_args) + " " + *it;

            for (it; it != args.end(); ++it) {
                res += ", " + getString(*it_args) + " " + *it;
                ++it_args;
            }
        }

        res += ")";
        return res;
    }

    std::string getString(ast::function& e) {
        std::string f_name = removeInvalidChars(e.function_name);
        processor::Types f_type = processor::function_types[f_name];

        std::string res = getString(f_type) + " " + f_name + getString(f_name, e.args);
        res += " {\n";
        res += getString(e.body);
        res += getString(e.return_);
        res += "}\n";

        return res;
    }

    std::string getString(ast::function_list& e) {

        std::string res = "";
        for (auto it = e.begin(); it != e.end(); ++it)
            res += getString(it->second);
        return res;
    }
}

#endif