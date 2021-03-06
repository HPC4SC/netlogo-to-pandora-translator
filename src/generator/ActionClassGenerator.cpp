#ifndef ACTION_GEN
#define ACTION_GEN

#include "StatementGenerator.cpp"

namespace generator {

    std::string generateExecute (std::string actionName, ast::statement_list body) {
        std::string ret = "void " + actionName + "::execute( Engine::Agent & agent ) {\n";
        ret += "Turtle& turtleAgent = (Turtle&)agent;\n";

        context = "turtleAgent.";
        ret += getString(body);
        context = "";

        return ret + "}\n";
    }

    std::string generateClass(std::string actionName, ast::statement_list body) {
        std::string ret = "";
        ret += actionName + "::" + actionName + "() {}\n";
        ret += actionName + "::~" + actionName + "() {}\n";
        ret += generateExecute(actionName, body);
        return ret;
    }

    std::string generateHeader(std::string actionName) {
        std::string ret = "class " + actionName + " : public Engine::Action {\n";
        ret += "public:\n";
        ret += actionName + "();\n";
        ret += "~" + actionName + "();\n";
        ret += "void execute( Engine::Agent & agent );";
        ret += "};\n";
        return ret;
    }

    void generateActionHeader (std::string actionName, std::string outputDir) {
        std::string output =    "#ifndef __" + actionName + "_hxx__\n"
                                "#define __" + actionName + "_hxx__\n"
                                "#include <Action.hxx>\n"
                                "#include <Agent.hxx>\n"
                                "#include <string>\n";

        output += "namespace Engine { class Agent; }\n";
        output += "namespace Examples {\n";

        output += generateHeader(actionName);

        output += "}\n";
        output += "#endif\n";

        std::ofstream myfile;
        myfile.open(outputDir + "/" + actionName + ".hxx");
        myfile << output;
        myfile.close();
    }

    void generateActionSource (std::string actionName, ast::statement_list body, std::string outputDir) {
        std::string output =    "#include <Action0.hxx>\n"
                                "#include <Turtle.hxx>\n"
                                "#include <string>\n";

        output += "namespace Engine { class Agent; }\n";
        output += "namespace Examples {\n";

        output += generateClass(actionName, body);

        output += "}\n";

        std::ofstream myfile;
        myfile.open(outputDir + "/" + actionName + ".cxx");
        myfile << output;
        myfile.close();
    }


    void generateAction (std::string actionName, ast::statement_list body, std::string outputDir) {
        generateActionHeader(actionName, outputDir);
        generateActionSource(actionName, body, outputDir);
    }
}

#endif