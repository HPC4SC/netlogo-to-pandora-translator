#ifndef AGENT_GEN
#define AGENT_GEN

#include "../processor/AgentActions.cpp"

namespace generator {

    std::string getClassInit() {
        std::string output =    "#ifndef __TurtleAgent_hxx__\n"
                                "#define __TurtleAgent_hxx__\n"
                                "#include <Globals.hxx>\n"
                                "#include <Agent.hxx>\n"
                                "#include <string>\n";
        
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            output += "#include <" + it->first + ".cxx>\n";
        }

        return output;
    }

    std::string getClassEnd() {
        return "#endif";
    }

    std::string getAttributes(ast::agent myAgent) {
        std::string result = "";

        // Common attributes
        result += "int angle = 0;\n";

        // Defined attributes
        for (auto it = myAgent.attributes.begin(); it != myAgent.attributes.end(); ++it) {
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

    std::string getSelectActions() {
        std::string ret = "void selectActions() {\n";

        // for each element on the actions list:
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            ret += "_actions.push_back(new " + it->first + "());\n";
        }

        return ret + "}\n";
    }

    std::string getCommonActions () {
        std::string ret = "";

        ret +=  "void setxy(int x, int y) {\n"
                "   Engine::Point2D<int> pos = getPosition();\n"
                "   pos._x = x;\n"
                "   pos._y = y;\n"
                "   setPosition(pos);\n"
                "}\n";

        ret +=  "void mv(int dir, int dist) {\n"
                "   Engine::Point2D<int> pos = getPosition();\n"
                "   switch(dir) {\n"
                "       case 0: angle -= dist; angle %= 360; break;\n"
                "       case 1: angle += dist; angle %= 360; break;\n"
                "       case 3: pos._x += cos(angle) * dist; pos._y += sin(angle) * dist; break;\n"
                "       case 4: pos._x -= cos(angle) * dist; pos._y -= sin(angle) * dist; break;\n"
                "   }\n"
                "   setPosition(pos);\n"
                "}\n";

        return ret;
    }

    std::string getAuxFunctions() {
        std::string ret = "";
        for (auto it = processor::agent_aux_functions.begin(); it != processor::agent_aux_functions.end(); ++it) {
            std::string f_name = *it;
            ast::function f = f_list[f_name];
            ret += generator::getString(f);
        }
        return ret;
    }

    std::string generateClass(ast::agent myAgent) {
        std::string ret = "class Turtle : public Engine::Agent {\n";
        ret += "public:\n";
        ret += getAttributes(myAgent);
        ret += "Turtle(const std::string & id) : Agent(id) {}\n";
        ret += "~Turtle() {}\n";
        ret += getSelectActions();
        ret += getCommonActions();
        ret += getAuxFunctions();
        ret += "};\n";
        return ret;
    }

    void generateTurtle (ast::agent myAgent) {
        std::string output = getClassInit() + generateClass(myAgent) + getClassEnd();

        std::ofstream myfile;
        myfile.open("build/Turtle.cxx");
        myfile << output;
        myfile.close();
    }
}

#endif