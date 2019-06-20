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
            "   std::string agentId = \"Turtle_\" + std::to_string(getId());\n"
            "   Turtle * agent = new Turtle(agentId);\n"
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

    void generate_world(ast::create_agentset& agentset_setup) {
        std::string result = 
            "#ifndef __World_hxx__\n"
            "#define __World_hxx__\n"

            "#include <Turtle.cxx>\n"

            "namespace Examples \n"
            "{\n"

            "class WorldConfig;\n"

            "class World : public Engine::World\n"
            "{\n";

        result += "    void createRasters() {\n";
        result += createRasters();
        result += "}\n";

        result += "    void createAgents() {\n";
        result += createAgents(agentset_setup);
        result += "}\n";

        result +=
            "    void stepEnvironment();\n"
            "public:\n"
            "    Earth( EarthConfig * config, Engine::Scheduler * scheduler = 0);\n"
            "    virtual ~Earth();\n"
            "};\n"

            "}\n"

            "#endif\n";


        std::ofstream myfile;
        myfile.open("build/World.cxx");
        myfile << result;
        myfile.close();
    }
}

#endif