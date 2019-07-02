#ifndef MAIN_FILE_GEN
#define MAIN_FILE_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    std::string generateGlobalVarsInitialization (ast::global_list& globals) {
        std::string result = "";
        for (auto it = globals.begin(); it != globals.end(); ++it) {
            std::string name = removeInvalidChars(*it);

            processor::Types t = processor::global_variable_types[*it];
            switch(t) {
                case processor::string_type: result += "std::string " + name + ";\n"; break;
                case processor::double_type: result += "double " + name + ";\n"; break;
                case processor::bool_type: result += "bool " + name + ";\n"; break;
                default: result += "auto " + name + ";\n"; break;
            }
        }
        return result;
    }

    void generate_main(ast::global_list& globals, std::string outputDir) {
        std::string result = 
            "#include <Exception.hxx>\n"
            "#include <iostream>\n"
            "#include \"GeneratedWorld.hxx\"\n"
            "#include \"Globals.hxx\"\n"
            "#include <Config.hxx>\n";

            result += generateGlobalVarsInitialization(globals);
            result += getString(f_list["setup-constants"]);

        result +=
            "int main(int argc, char *argv[])\n"
            "{\n"
            "    try\n"
            "    {\n";
        result += 
            "        setupconstants();"

            "        Engine::Config *worldConfig = new Engine::Config(Engine::Size<int>(100, 100), 500);\n"
            "        Examples::GeneratedWorld world(worldConfig);\n"

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
        myfile.open(outputDir + "/main.cxx");
        myfile << result;
        myfile.close();
    }
}

#endif