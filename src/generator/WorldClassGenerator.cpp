#ifndef WORLD_GEN
#define WORLD_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"
#include "ExpressionGenerator.cpp"

#include <fstream>

namespace generator {

    std::string createRasters() {
        return "";
    }

    std::string createTurtles(ast::expression quantity) {
        std::string res = 
            "for(int i = 0; i < " + getString(quantity) + "; ++i) {\n"
            "   Turtle * agent = new Turtle(\"Agent_\" + std::to_string(i));\n"
            "   addAgent(agent);\n"
            "}\n";

        return res;
    }

    std::string createAgents(ast::create_agentset& agentset_setup) {
        switch(agentset_setup.type) {
            case ast::turtle: return createTurtles(agentset_setup.quantity);
        }
        return "";
    }

    void generateHeader () {
        std::string result = 
            "#ifndef __Gen_World_hxx__\n"
            "#define __Gen_World_hxx__\n"

            "#include <Turtle.hxx>\n"

            "namespace Examples \n"
            "{\n"

            "class GeneratedWorldConfig;\n"

            "class GeneratedWorld : public Engine::World\n"
            "{\n";

        result += "void createRasters();\n";
        result += "void createAgents();\n";

        result +=
            "void stepEnvironment();\n"
            "public:\n"
            "GeneratedWorld( Engine::Config * config, Engine::Scheduler * scheduler = 0);\n"
            "virtual ~GeneratedWorld();\n"
            "};\n"

            "}\n"

            "#endif\n";


        std::ofstream myfile;
        myfile.open("build/GeneratedWorld.hxx");
        myfile << result;
        myfile.close();
    }

    void generateSource (ast::create_agentset& agentset_setup) {
        std::string result = 
            "#include <GeneratedWorld.hxx>\n"
            "#include <Turtle.hxx>\n"
            "#include <Globals.hxx>\n"
            "namespace Examples \n"
            "{\n";
        result += "GeneratedWorld::GeneratedWorld (Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false) {}\n";
        result += "GeneratedWorld::~GeneratedWorld() {}\n";

        result += "void GeneratedWorld::createRasters() {\n";
            result += createRasters();
        result += "}\n";

        result += "void GeneratedWorld::createAgents() {\n";
            result += createAgents(agentset_setup);
        result += "}\n";
        
        result += "void GeneratedWorld::stepEnvironment() {}\n";
        result += "}\n";



        std::ofstream myfile;
        myfile.open("build/GeneratedWorld.cxx");
        myfile << result;
        myfile.close();
    }

    void generate_world(ast::create_agentset& agentset_setup) {
        generateHeader();
        generateSource(agentset_setup);
    }
}

#endif