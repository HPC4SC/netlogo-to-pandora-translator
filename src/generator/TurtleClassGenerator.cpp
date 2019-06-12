#ifndef AGENT_GEN
#define AGENT_GEN

namespace generator {

    std::string getClassInit() {
        std::string output =    "#ifndef __TurtleAgent_hxx__\n"
                                "#define __TurtleAgent_hxx__\n"
                                "#include <Agent.hxx>\n"
                                "#include <string>\n";
        // for each element on the actions list:
        // output += "#include <" + actionName + ".cxx>\n";
        return output;
    }

    std::string getClassEnd() {
        return "#endif";
    }

    std::string getAttributes(ast::agent myAgent) {
        std::string result = "";
        for (auto it = myAgent.attributes.begin(); it != myAgent.attributes.end(); ++it) {
            std::string name = removeInvalidChars(*it);

            inference::Types t = inference::global_variable_types[*it];
            switch(t) {
                case inference::string_type: result += "std::string " + name + ";\n"; break;
                case inference::double_type: result += "double " + name + ";\n"; break;
                case inference::bool_type: result += "bool " + name + ";\n"; break;
                default: result += "auto " + name + ";\n"; break;
            }
        }
        return result;
    }

    std::string getSelectActions() {
        std::string ret = "void selectActions() {\n";

        // for each element on the actions list:
        // ret += "_actions.push_back(new " + actionName + "());"

        return ret + "}\n";
    }

    std::string generateClass(ast::agent myAgent) {
        std::string ret = "class Turtle : public Engine::Agent {\n";
        ret += "public:\n"
        ret += getAttributes(myAgent);
        ret += "Turtle(const std::string & id) : Agent(id) {}\n";
        ret += "~Turtle() {}\n";
        ret += getSelectActions();
        ret += "};\n";
        return ret;
    }

    void generateTurtle (ast::agent myAgent) {
        std::string output = getClassInit() + generateClass(myAgent) + getClassEnd();

        std::ofstream myfile;
        myfile.open("build/Turtle.cpp");
        myfile << output;
        myfile.close();
    }
}

#endif