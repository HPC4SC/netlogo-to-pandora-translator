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
    
    int actionId = 0;
    std::map<std::string, ast::ask_agentset> agent_actions;
    std::queue<std::string> func_queue;

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
        void operator()(const ast::function_call& e) const {
            func_queue.push(e.function_name);
            std::cout << e.function_name << std::endl;
        }
        void operator()(const ast::ask_agentset& e) const { 
            agent_actions["Action" + actionId] = e;
            std::cout << "Action: " << actionId << std::endl;
        }
        void operator()(const ast::ask_agent& e) const { 
            std::cout << "Action 2: " << actionId << std::endl;
        }
    };

    void findAgentActions(ast::main& e) {
        FindAgentAction findAgentAction;
        func_queue.push("go");
        std::cout << "go"<< std::endl;
        while (!func_queue.empty()) {
            std::cout << "a"<< std::endl;
            std::string f_name = func_queue.front();
            ast::function f = e.functions[f_name];
            std::cout << "Size: " << f.body.size() << std::endl;
            BOOST_FOREACH(ast::statement const& st, f.body)
                findAgentAction(st);
            func_queue.pop();
        }
    }
}

#endif