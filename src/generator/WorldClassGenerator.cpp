#ifndef WORLD_GEN
#define WORLD_GEN

#include "../parser/AST.hpp"
#include "../processor/TypeInference.cpp"
#include "ExpressionGenerator.cpp"

#include <fstream>

namespace generator {

    struct world_statement_visitor : boost::static_visitor<std::string>
    {
        std::string operator()(const ast::variable_declaration& e) const { return getString(e); }
        std::string operator()(const ast::assignment& e) const { return getString(e); }
        std::string operator()(const ast::single_word_statement& e) const { return getString(e); }
        std::string operator()(const ast::setxy_statement& e) const { return getString(e); }
        std::string operator()(const ast::function_call& e) const { return getString(e); }
        std::string operator()(const ast::ask_agentset& e) const { return generateTurtleExprFromWorld(e); }
        std::string operator()(const ast::ask_agent& e) const { return getString(e); }
        std::string operator()(const ast::create_agentset& e) const { return getString(e); }
        std::string operator()(const ast::move_statement& e) const { return getString(e); }
        std::string operator()(const ast::if_statement& e) const { return getString(e); }
        std::string operator()(const ast::while_statement& e) const { return getString(e); }
        std::string operator()(const ast::statement_list& e) const { return getString(e); }
    };

    std::string createRasters() {
        return "";
    }

    std::string createTurtles(ast::create_agentset& agentset_setup) {
        std::string res = 
            "for(int i = 0; i < " + getString(agentset_setup.quantity) + "; ++i) {\n"
            "   Turtle * agent = new Turtle(\"Agent_\" + std::to_string(i));\n"
            "   addAgent(agent);\n";
        context = "agent->";
        res += generator::getString(agentset_setup.body);
        context = "agent->";
        res += "}\n";

        return res;
    }

    std::string createAgents(ast::create_agentset& agentset_setup) {
        switch(agentset_setup.type) {
            case ast::turtle: return createTurtles(agentset_setup);
        }
        return "";
    }

    std::string getTurtleExtraInitialization(const ast::statement_list& st) {
        std::string result = "";
        for (auto it = st.begin(); it != st.end(); ++it) {
            result += boost::apply_visitor(world_statement_visitor(), *it);
        }
        return result;
    }

    void generateHeader () {
        std::string result = 
            "#ifndef __Gen_World_hxx__\n"
            "#define __Gen_World_hxx__\n"

            "#include <World.hxx>\n"
            "#include \"Globals.hxx\"\n"

            "namespace Engine\n"
            "{\n"
            "    class Config;\n"
            "}\n"

            "namespace Examples \n"
            "{\n"

            "class GeneratedWorld : public Engine::World\n"
            "{\n";

        result += "void createRasters();\n";
        result += "void createAgents();\n";

        result +=
            "public:\n"
            "GeneratedWorld( Engine::Config * config, Engine::Scheduler * scheduler = 0);\n"
            "virtual ~GeneratedWorld();\n"
            "};\n"

            "}\n"

            "#endif\n";


        std::ofstream myfile;
        myfile.open("build/GeneratedWorld.hxx");
        myfile << result;
        myfile.close();
    }

    void generateSource (ast::create_agentset& agentset_setup) {
        ast::function turtles_setup = f_list["setup-turtles"];

        std::string result = 
            "#include \"GeneratedWorld.hxx\"\n"
            
            "#include <Config.hxx>\n"
            "#include \"Turtle.hxx\"\n"
            "#include <Globals.hxx>\n"
            "namespace Examples \n"
            "{\n";
        result += "GeneratedWorld::GeneratedWorld (Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false) {}\n";
        result += "GeneratedWorld::~GeneratedWorld() {}\n";

        result += "void GeneratedWorld::createRasters() {\n";
            result += createRasters();
        result += "}\n";

        result += "void GeneratedWorld::createAgents() {\n";
            result += createAgents(agentset_setup);
            result += getTurtleExtraInitialization(turtles_setup.body);
        result += "}\n";
        
        result += "}\n";



        std::ofstream myfile;
        myfile.open("build/GeneratedWorld.cxx");
        myfile << result;
        myfile.close();
    }

    void generate_world(ast::create_agentset& agentset_setup) {
        generateHeader();
        generateSource(agentset_setup);
    }
}

#endif