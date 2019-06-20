#ifndef TURTLE_CPP
#define TURTLE_CPP

#include "StatementGenerator.cpp"
#include "../processor/AgentActions.cpp"
#include <fstream>

class Turtle {
private:
    // Definitions
    std::string startDefinition;
    std::string endDefinition;

    // Includes
    std::string includes; // S'ha de generar, ja que s'han d'incloure les Actions

    std::string attributes;

    std::string constructor;

    std::string selectActions;
    std::string commonActions;
    std::string auxFunctions;

public:
    Turtle () {
        startDefinition =   "#ifndef __TurtleAgent_hxx__\n"
                            "#define __TurtleAgent_hxx__\n";
        endDefinition = "#endif";

        commonActions = "void setxy(int x, int y) {\n"
                        "   Engine::Point2D<int> pos = getPosition();\n"
                        "   pos._x = x;\n"
                        "   pos._y = y;\n"
                        "   setPosition(pos);\n"
                        "}\n"
                        "void mv(int dir, int dist) {\n"
                        "   Engine::Point2D<int> pos = getPosition();\n"
                        "   switch(dir) {\n"
                        "       case 0: angle -= dist; angle %= 360; break;\n"
                        "       case 1: angle += dist; angle %= 360; break;\n"
                        "       case 3: pos._x += cos(angle) * dist; pos._y += sin(angle) * dist; break;\n"
                        "       case 4: pos._x -= cos(angle) * dist; pos._y -= sin(angle) * dist; break;\n"
                        "   }\n"
                        "   setPosition(pos);\n"
                        "}\n";
    }
    ~Turtle () {}

    void generateIncludes() {
        includes =  "#include <Globals.cxx>\n"
                    "#include <Agent.hxx>\n"
                    "#include <string>\n";
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            includes += "#include <" + it->first + ".cxx>\n";
        }
    }

    void generateAttributes(ast::agent& myAgent) {
        // Common attributes
        attributes = "int angle = 0;\n";

        // Defined attributes
        for (auto it = myAgent.attributes.begin(); it != myAgent.attributes.end(); ++it) {
            std::string name = removeInvalidChars(*it);

            processor::Types t = processor::global_variable_types[*it];
            switch(t) {
                case processor::string_type: attributes += "std::string " + name + ";\n"; break;
                case processor::double_type: attributes += "double " + name + ";\n"; break;
                case processor::bool_type: attributes += "bool " + name + ";\n"; break;
                default: attributes += "auto " + name + ";\n"; break;
            }
        }
    }

    void generateConstructor (ast::create_agentset& create) {
        constructor = "Turtle(const std::string & id) : Agent(id) {\n";
        constructor += generator::getString(create.body);
        constructor += "}\n";
    }

    void generateSelectActions() {
        selectActions = "void selectActions() {\n";

        // for each element on the actions list:
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            selectActions += "_actions.push_back(new " + it->first + "());\n";
        }

        selectActions += "}\n";
    }

    void generateAuxFunctions() {
        auxFunctions = "";
        for (auto it = processor::agent_aux_functions.begin(); it != processor::agent_aux_functions.end(); ++it) {
            std::string f_name = *it;
            ast::function f = f_list[f_name];
            auxFunctions += generator::getString(f);
        }
    }

    void generate () {
        std::string output = "";

        output += startDefinition;
        output += includes;
        output += "namespace Examples {\n";
        output += "class Turtle : public Engine::Agent {\n";
        output += "public:\n";
        output += attributes;
        output += selectActions;
        output += commonActions;
        output += auxFunctions;
        output += constructor;
        output += "~Turtle() {}\n";
        output += "};\n";
        output += "}\n";
        output += endDefinition;

        std::ofstream myfile;
        myfile.open("build/Turtle.cxx");
        myfile << output;
        myfile.close();
    }
};

#endif