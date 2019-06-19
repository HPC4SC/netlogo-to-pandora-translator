#ifndef ACTION_GEN
#define ACTION_GEN

#include "StatementGenerator.cpp"

namespace generator {

    std::string generateExecute (ast::statement_list body) {
        std::string ret = "void execute( Engine::Agent & agent ) {\n";
        ret += "Turtle& turtleAgent = (Turtle&)agent;\n";
        context = "agent.";
        ret += getString(body);
        context = "";

        return ret + "}\n";
    }

    std::string generateClass(std::string actionName, ast::statement_list body) {
        std::string ret = "class " + actionName + " : public Engine::Action {\n";
        ret += "public:\n";
        ret += actionName + "() {}\n";
        ret += "~" + actionName + "() {}\n";

        /*
            AuxiliaryFunctions s'han de moure a la clase Turtle i de manera recursiva buscar totes les functionCalls
            dins de tots els ask.

            A les auxiliaryFunctions, si es troba un ask s'ha de fer un loop per les turtles i afegir el seu 
            context (turtle.function_name(), per exemple)

            El generateExecute només ha de cridar a les funcions de la turtle,
            per tant s'ha de afegir el context del agent a cada crida/variable que s'accedeixi (agent.function_name(), per exemple)
        */

        ret += generateExecute(body);
        ret += "};\n";
        return ret;
    }

    void generateAction (std::string actionName, ast::statement_list body) {
        std::string output =    "#ifndef __" + actionName + "_hxx__\n"
                                "#define __" + actionName + "_hxx__\n"
                                "#include <Action.hxx>\n"
                                "#include <Agent.hxx>\n"
                                "#include <Turtle.cxx>\n"
                                "#include <string>\n";

        output += "namespace Engine { class Agent; }\n";

        output += generateClass(actionName, body);

        output += "#endif\n";

        std::ofstream myfile;
        myfile.open("build/" + actionName + ".cxx");
        myfile << output;
        myfile.close();
    }
}

#endif