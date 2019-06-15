/*

1. Find an agent action (ask turtles ...)
2. Create a class with the action contents
3. Fill the data structure with information about that class (name)

*/ 

#ifndef AGENT_ACTIONS_CPP
#define AGENT_ACTIONS_CPP

#include "../parser/AST.hpp"

#include <queue>

namespace processor {

    std::string getNewActionId () {
        std::string action_name = "Action" + std::to_string(actionId);
        ++actionId;
        return action_name;
    }

    void findAuxFunctions (const ast::statement_list& s_list) {          
        for (auto it = s_list.begin(); it != s_list.end(); ++it) {
            if ((*it).which() == 4) { // Id 4 is the function_call id inside the statement variant
                ast::function_call f_call = boost::get<ast::function_call>(*it);
                std::string f_name = f_call.function_name;
                agent_aux_functions.push_back(f_name);
            }
        }
    }

    struct FindAgentAction : boost::static_visitor<void> {
        FindAgentAction() {}

        void operator()(const ast::variable_declaration& e) const { }
        void operator()(const ast::assignment& e) const { }
        void operator()(const ast::single_word_statement& e) const { }
        void operator()(const ast::setxy_statement& e) const { }
        void operator()(const ast::create_agentset& e) const { }
        void operator()(const ast::move_statement& e) const { }
        void operator()(const ast::if_statement& e) const { }
        void operator()(const ast::while_statement& e) const { }
        void operator()(const ast::statement_list& e) const { }
        void operator()(const ast::function_call& e) const { }
        void operator()(const ast::ask_agentset& e) const { 
            std::string action_name = getNewActionId();
            agent_actions[action_name] = e;
            findAuxFunctions(e.body);
        }
        void operator()(const ast::ask_agent& e) const { 
            // TODO
            findAuxFunctions(e.body);
        }
    };

    void findAgentActions(ast::function& go_function) {
        FindAgentAction findAgentAction;
        for (auto it = go_function.body.begin(); it != go_function.body.end(); ++it) {
            boost::apply_visitor(findAgentAction, *it);
        }
    }

    void scanAgentActions(ast::main& e) {
        // Create identifiers for new actions to be generated later
        findAgentActions(e.functions["go"]);
    }
}

#endif