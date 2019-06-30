#ifndef TURTLE_CPP
#define TURTLE_CPP

#include "StatementGenerator.cpp"
#include "../processor/AgentActions.cpp"
#include <fstream>

class Turtle {
private:
    std::string functions;

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
        functions = "void setxy(int x, int y);\n"
                    "void mv(int dir, int dist);\n";

        startDefinition =   "#ifndef __TurtleAgent_hxx__\n"
                            "#define __TurtleAgent_hxx__\n";
        endDefinition = "#endif";

        commonActions = "void Turtle::setxy(int x, int y) {\n"
                        "   Engine::Point2D<int> newPos = _position;\n"
                        "   newPos._x = x;\n"
                        "   newPos._y = y;\n"
                        "   if (_world->checkPosition(newPos))\n"
                        "   {\n"
                        "       setPosition(newPos);\n"
                        "   }\n"
                        "}\n"
                        "void Turtle::mv(int dir, int dist) {\n"
                        "   Engine::Point2D<int> pos = _position;\n"
                        "   switch(dir) {\n"
                        "       case 0: angle -= dist; if(angle < 0) angle = 360 - angle; break;"
                        "       case 1: angle += dist; angle %= 360; break;\n"
                        "       case 2: pos._x += std::round(cos(angle*3.14/180) * dist); pos._y += std::round(sin(angle*3.14/180) * dist); break;\n"
                        "       case 3: pos._x -= std::round(cos(angle*3.14/180) * dist); pos._y -= std::round(sin(angle*3.14/180) * dist); break;\n"
                        "   }\n"
                        "   if (_world->checkPosition(pos))\n"
                        "   {\n"
                        "       setPosition(pos);\n"
                        "   }\n"
                        "   else {\n"
                        "       if (pos._x < 0) pos._x = 100 + pos._x;\n"
                        "       if (pos._x > 100) pos._x %= 100;\n"
                        "       if (pos._y < 0) pos._y = 100 + pos._y;\n"
                        "       if (pos._y > 100) pos._y %= 100;\n"
                            
                        "       setPosition(pos);\n"
                        "   }\n"
                        "}\n"
                        "void Turtle::reproduce()\n"
                        "{\n"
                        "    if (_world->getNumberOfAgents() < carryingcapacity && random(100) < chancereproduce)\n"
                        "    {\n"
                        "        for (int i = 0; i < 2; ++i)\n"
                        "        {\n"
                        "            Turtle *child = new Turtle(_id + \"_\" + std::to_string(i));\n"
                        "            _world->addAgent(child);\n"
                        "            child->age = 1;\n"
                        "            child->mv(0, 45);\n"
                        "            child->mv(2, 1);\n"
                        "            child->gethealthy();\n"
                        "        }\n"
                        "    }\n"
                        "};\n";
    }
    ~Turtle () {}

    void generateIncludes() {
        includes =  "#include <Agent.hxx>\n"
                    "#include <string>\n";

    }

    void generateConstructor () {
        constructor = "Turtle::Turtle(const std::string & id) : Agent(id) {\n";
        constructor += "    _exists = true;\n";
        constructor += "    angle = random(360);\n";
        constructor += "}\n";
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

    void generateSelectActions() {
        selectActions = "void Turtle::selectActions() {\n";

        // for each element on the actions list:
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            selectActions += "_actions.push_back(new " + it->first + "());\n";
        }

        selectActions += "}\n";
    }

    std::string generateFunction(ast::function& e) {
        processor::Types f_type = processor::function_types[e.function_name];
        std::string f_name = removeInvalidChars(e.function_name);

        std::string res = generator::getString(f_type) + " Turtle::" + f_name + generator::getString(f_name, e.args);
        res += " {\n";
        res += generator::getString(e.body);
        res += generator::getString(e.return_);
        res += "}\n";

        return res;
    }

    void generateAuxFunctions() {
        auxFunctions = "";
        for (auto it = processor::agent_aux_functions.begin(); it != processor::agent_aux_functions.end(); ++it) {
            std::string f_name = *it;
            if (f_name == "reproduce") continue;
            ast::function f = f_list[f_name];
            auxFunctions += generateFunction(f);
            functions += generator::getFunctionHeader(f);
        }
    }

    void generateHeaderFile () {
        std::string output = "";

        output += startDefinition;
        output += "#include <Agent.hxx>\n";
        output += "#include \"Globals.hxx\"\n";
        output += "#include <string>\n";
        output += "namespace Examples \n{\n";
        output += "class Turtle : public Engine::Agent \n{\n";
        output += "public:\n";
        output += "Turtle(const std::string & id);\n";
        output += "~Turtle();\n";
        output += "void selectActions();\n";
        output += "void reproduce();\n";
        output += attributes;
        output += functions;
        output += "};\n";
        output += "}\n";
        output += endDefinition;

        std::ofstream myfile;
        myfile.open("build/Turtle.hxx");
        myfile << output;
        myfile.close();
    }

    void generateSourceFile () {
        std::string output = "";

        output += "#include \"Turtle.hxx\"\n";
        output += includes;
        for (auto it = processor::agent_actions.begin(); it != processor::agent_actions.end(); ++it) {
            output += "#include \"" + it->first + ".hxx\"\n";
        }
        output += "namespace Examples {\n";
        output += constructor;
        output += "Turtle::~Turtle() {}\n";
        output += selectActions;
        output += commonActions;
        output += auxFunctions;
        output += "}\n";

        std::ofstream myfile;
        myfile.open("build/Turtle.cxx");
        myfile << output;
        myfile.close();
    }

    void generate () {
        generateHeaderFile();
        generateSourceFile();
    }
};

#endif