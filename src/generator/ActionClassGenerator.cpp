#ifndef ACTION_GEN
#define ACTION_GEN

#include "StatementGenerator.cpp"

namespace generator {

    std::string generateExecute (ast::statement_list body) {
        std::string ret = "void execute( Engine::Agent & agent ) {\n";
        ret += getString(body);
        return ret + "}\n";
    }

    std::string getAxiliaryFunctions (ast::statement_list body) {

        // This function will look at any function_call on the body of the action
        // if exists any function call, it will include it in the code of the class
        
        return "";
    }

    std::string generateClass(std::string actionName, ast::statement_list body) {
        std::string ret = "class " + actionName + " : public Engine::Action {\n";
        ret += "public:\n";
        ret += actionName + "(const std::string & id) : Agent(id) {}\n";
        ret += "~" + actionName + "() {}\n";
        ret += generateExecute(body);
        ret += getAxiliaryFunctions(body);
        ret += "};\n";
        return ret;
    }

    void generateAction (std::string actionName, ast::statement_list body) {
        std::string output =    "#ifndef __" + actionName + "_hxx__\n"
                                "#define __" + actionName + "_hxx__\n"
                                "#include <Action.hxx>\n"
                                "#include <Agent.hxx>\n"
                                "#include <string>\n";

        output += "namespace Engine { class Agent; }\n";

        output += generateClass(actionName, body);

        output += "#endif\n";

        std::ofstream myfile;
        myfile.open("build/" + actionName + ".cpp");
        myfile << output;
        myfile.close();
    }
}

#endif