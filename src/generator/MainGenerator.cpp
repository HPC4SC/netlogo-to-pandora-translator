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
#include "TurtleClassGenerator.cpp"
#include "ActionClassGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../processor/AgentActions.cpp"
#include "../parser/AST.hpp"

namespace generator {

    void generate_config(ast::configuration& e) {
        generate(e.globals);

        for (auto it = e.agents.begin(); it != e.agents.end(); ++it) {
            switch(it->type) {
                case ast::turtle: generateTurtle(*it); break;
                case ast::patch: break;
            }
        }
    }

    void generate_setup(ast::function& f) {
        
    }

    void generate_go(ast::function& f) {

        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            generateAction(it->first, (it->second).body);
        }
    }

    void generate(ast::main& e) {
        generate_config(e.config);

        if (e.functions.find("setup") != e.functions.end())
            generate_setup(e.functions["setup"]);

        if (e.functions.find("go") != e.functions.end())
            generate_go(e.functions["go"]);
    }

}

#endif