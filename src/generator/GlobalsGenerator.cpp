#ifndef GLOBALS_GEN
#define GLOBALS_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    std::string generateGlobalFunctions () {
        std::string res = "";

        res += "int random(int max) { return rand() % (max + 1); }\n";
        res += "int randomxcor() { return rand() % 101; }\n";
        res += "int randomycor() { return rand() % 101; }\n";

        return res;
    }

    std::string generateGlobalVars (ast::global_list& globals) {
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

    void generate(ast::global_list& globals) {
        std::string result =    "#ifndef __Globals_hxx__\n"
                                "#define __Globals_hxx__\n"
                                "#include <stdlib.h>\n";
        
        result += generateGlobalVars(globals);
        result += generateGlobalFunctions();

        result += "#endif\n";

        std::ofstream myfile;
        myfile.open("build/Globals.cxx");
        myfile << result;
        myfile.close();
    }
}

#endif