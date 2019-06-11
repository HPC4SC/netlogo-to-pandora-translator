#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "StatementGenerator.cpp"
#include "TypeInference.cpp"
#include "AST.hpp"

namespace generator {
    std::string getString(inference::Types type) {
        switch(type) {
            case inference::string_type: return "std::string";
            case inference::double_type: return "double";
            case inference::bool_type: return "bool";
            case inference::void_type: return "void";
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
            std::list<inference::Types> arg_types = inference::function_args_types[f_name];

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
        std::string f_name = e.function_name;
        inference::Types f_type = inference::function_types[f_name];

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
            res += getString(*it);
        return res;
    }
}