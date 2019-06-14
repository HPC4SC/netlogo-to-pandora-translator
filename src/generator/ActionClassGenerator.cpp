#ifndef ACTION_GEN
#define ACTION_GEN

#include "StatementGenerator.cpp"

namespace generator {

    std::string generateExecute (ast::statement_list body) {
        std::string ret = "void execute( Engine::Agent & agent ) {\n";
        ret += getString(body);
        return ret + "}\n";
    }

    std::string getAuxiliaryFunctions (ast::statement_list body) {
        std::string ret = "";

        for (auto it = body.begin(); it != body.end(); ++it) {
            if ((*it).which() == 4) { // Id 4 is the function_call id inside the statement variant
                ast::function_call f_call = boost::get<ast::function_call>(*it);
                std::string f_name = f_call.function_name;
                ast::function f = parser::f_list[f_name];

                ret += getAuxiliaryFunctions(f.body); // Recursively search for more auxiliary functions
                ret += getString(f);
            }
        }
        
        return ret;
    }

    std::string generateClass(std::string actionName, ast::statement_list body) {
        std::string ret = "class " + actionName + " : public Engine::Action {\n";
        ret += "public:\n";
        ret += actionName + "(const std::string & id) : Agent(id) {}\n";
        ret += "~" + actionName + "() {}\n";

        /*
            AuxiliaryFunctions s'han de moure a la clase Turtle i de manera recursiva buscar totes les functionCalls
            dins de tots els ask.

            A les auxiliaryFunctions, si es troba un ask s'ha de fer un loop per les turtles i afegir el seu 
            context (turtle.function_name(), per exemple)

            El generateExecute només ha de cridar a les funcions de la turtle,
            per tant s'ha de afegir el context del agent a cada crida/variable que s'accedeixi (agent.function_name(), per exemple)
        */

        //ret += getAuxiliaryFunctions(body);
        ret += generateExecute(body);
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
        myfile.open("build/" + actionName + ".cxx");
        myfile << output;
        myfile.close();
    }
}

#endif