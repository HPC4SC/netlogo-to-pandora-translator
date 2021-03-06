#ifndef GLOBALS_GEN
#define GLOBALS_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"

#include <fstream>

namespace generator {

    std::string generateGlobalFunctions () {
        std::string res = "";

        res += "extern int random(int max);\n";
        res += "extern int randomxcor();\n";
        res += "extern int randomycor();\n";

        return res;
    }

    std::string generateGlobalVars (ast::global_list& globals) {
        std::string result = "";
        for (auto it = globals.begin(); it != globals.end(); ++it) {
            std::string name = removeInvalidChars(*it);

            processor::Types t = processor::global_variable_types[*it];
            switch(t) {
                case processor::string_type: result += "extern std::string " + name + ";\n"; break;
                case processor::double_type: result += "extern double " + name + ";\n"; break;
                case processor::bool_type: result += "extern bool " + name + ";\n"; break;
                default: result += "extern auto " + name + ";\n"; break;
            }
        }
        return result;
    }

    void generateHeader (ast::global_list& globals, std::string outputDir) {
        std::string result =    "#ifndef __Globals_hxx__\n"
                                "#define __Globals_hxx__\n"
                                "#include <stdlib.h>\n";
        
        result += generateGlobalVars(globals);
        result += generateGlobalFunctions();

        result += "#endif\n";

        std::ofstream myfile;
        myfile.open(outputDir + "/Globals.hxx");
        myfile << result;
        myfile.close();
    }

    void generateSource (std::string outputDir) {
        std::string source = 
            "#include \"Globals.hxx\"\n"
            "#include <stdlib.h>\n"

            "extern int random(int max) { return rand() % (max + 1); }\n"
            "extern int randomxcor() { return rand() % 101; }\n"
            "extern int randomycor() { return rand() % 101; }\n";

        std::ofstream myfile;
        myfile.open(outputDir + "/Globals.cxx");
        myfile << source;
        myfile.close();
    }

    void generate(ast::global_list& globals, std::string outputDir) {
        generateHeader(globals, outputDir);
        generateSource(outputDir);
    }
}

#endif