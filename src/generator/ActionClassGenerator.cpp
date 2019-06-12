#ifndef ACTION_GEN
#define ACTION_GEN

#include "StatementGenerator.cpp"

namespace generator {

    std::string getClassInit(std::string actionName) {
        std::string output =    "#ifndef __" + actionName + "_hxx__\n"
                                "#define __" + actionName + "_hxx__\n"
                                "#include <Action.hxx>\n"
                                "#include <Agent.hxx>\n"
                                "#include <string>\n";

        output += "namespace Engine { class Agent; }"
        return output;
    }

    std::string getClassEnd() {
        return "#endif";
    }

    std::string generateAction (ast::statement_list body) {
        std::string ret = "void execute( Engine::Agent & agent ) {\n";
        ret += getString(body);
        return ret + "}\n";
    }

    std::string generateClass(std::string actionName, ast::statement_list body) {
        std::string ret = "class " + actionName + " : public Engine::Action {\n";
        ret += "public:\n"
        ret += actionName + "(const std::string & id) : Agent(id) {}\n";
        ret += "~" + actionName + "() {}\n";
        ret += generateExecute(body);
        ret += "};\n";
        return ret;
    }

    void generateAction (std::string actionName, ast::statement_list body) {
        std::string output = getClassInit() + generateClass(actionName, body) + getClassEnd();

        std::ofstream myfile;
        myfile.open("build/" + actionName + ".cpp");
        myfile << output;
        myfile.close();
    }
}

#endif