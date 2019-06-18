#ifndef WORLD_GEN
#define WORLD_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    void generate_world() {
        std::string result = 
            "#ifndef __World_hxx__\n"
            "#define __World_hxx__\n"

            "#include <World.hxx>\n"

            "namespace Generated \n"
            "{\n"

            "class WorldConfig;\n"

            "class World : public Engine::World\n"
            "{\n"
            "    void createRasters();\n"
            "    void createAgents();\n"

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