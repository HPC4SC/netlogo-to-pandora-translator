#ifndef GLOBALS_GEN
#define GLOBALS_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    void generate(ast::global_list& globals) {
        std::string result = "";
        for (auto it = globals.begin(); it != globals.end(); ++it) {
            std::string name = removeInvalidChars(*it);

            inference::Types t = inference::global_variable_types[*it];
            switch(t) {
                case inference::string_type: result += "std::string " + name + ";\n"; break;
                case inference::double_type: result += "double " + name + ";\n"; break;
                case inference::bool_type: result += "bool " + name + ";\n"; break;
                default: result += "auto " + name + ";\n"; break;
            }
        }

        std::ofstream myfile;
        myfile.open("build/globals.cpp");
        myfile << result;
        myfile.close();
    }
}

#endif