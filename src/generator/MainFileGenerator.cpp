#ifndef MAIN_FILE_GEN
#define MAIN_FILE_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    void generate_main(std::string setup) {
        std::string result = 
            "#include <iostream>\n"
            "#include <cstdlib>\n"

            "#include <World.hxx>\n";

        for (auto it = processor::setup_aux_functions.begin(); it != processor::setup_aux_functions.end(); ++it) {
            ast::function f = f_list[*it];
            result += getString(f);
        }

        result +=
            "int main(int argc, char *argv[])\n"
            "{\n"
            "    try\n"
            "    {\n";
        result += setup;
        result += 
            "        std::string fileName(\"config.xml\");\n"

            "        Generated::World world( new Generated::WorldConfig(fileName), world.useOpenMPSingleNode());\n"

            "        world.initialize(argc,argv);\n"
            "        world.run();\n"
            "    }\n"
            "    catch( std::exception & exceptionThrown )\n"
            "    {\n"
            "        std::cout << \"exception thrown: \" << exceptionThrown.what() << std::endl;\n"
            "    }\n"
            "    return 0;\n"
            "}\n";

        std::ofstream myfile;
        myfile.open("build/main.cxx");
        myfile << result;
        myfile.close();
    }
}

#endif