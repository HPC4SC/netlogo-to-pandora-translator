#ifndef MAIN_GEN
#define MAIN_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "FunctionGenerator.cpp"
#include "GlobalsGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../parser/AST.hpp"

namespace generator {

    void generate(ast::configuration& e) {
        generate(e.globals);
        // generate the Agent constructor here
    }

    void generate(ast::main& e) {
        generate(e.config);
    }

}

#endif