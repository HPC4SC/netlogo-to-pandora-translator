#ifndef MAIN_GEN
#define MAIN_GEN

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>

#include "Turtle.cpp"
#include "MainFileGenerator.cpp"
#include "WorldClassGenerator.cpp"
#include "FunctionGenerator.cpp"
#include "GlobalsGenerator.cpp"
#include "ActionClassGenerator.cpp"
#include "../processor/TypeInference.cpp"
#include "../processor/AgentActions.cpp"
#include "../parser/AST.hpp"

namespace generator {

    void generate_config(ast::configuration& e, Turtle& turtle, std::string outputDir) {
        generate(e.globals, outputDir);

        for (auto it = e.agents.begin(); it != e.agents.end(); ++it) {
            switch(it->type) {
                case ast::turtle: {
                    turtle.generateAttributes(*it);
                    break;
                }
                case ast::patch: break;
            }
        }
    }

    void generate_setup(ast::global_list& globals, std::string outputDir) {
        generate_main(globals, outputDir);
    }

    void generate_go(ast::function& f, std::string outputDir) {

        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            generateAction(it->first, (it->second).body, outputDir);
        }
    }

    void generate(ast::main& e, std::string outputDir) {
        Turtle turtle(outputDir);
        turtle.generateIncludes();
        turtle.generateConstructor();
        turtle.generateSelectActions();
        turtle.generateAuxFunctions();

        generate_world(processor::agentset_setup, outputDir);
        generate_config(e.config, turtle, outputDir);

        if (e.functions.find("setup") != e.functions.end())
            generate_setup(e.config.globals, outputDir);

        if (e.functions.find("go") != e.functions.end())
            generate_go(e.functions["go"], outputDir);

        turtle.generate();
    }

}

#endif