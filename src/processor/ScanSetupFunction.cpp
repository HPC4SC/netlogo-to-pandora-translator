/*

1. Find an agent action (ask turtles ...)
2. Create a class with the action contents
3. Fill the data structure with information about that class (name)

*/ 

#ifndef SCAN_SETUP_FUNCTION_CPP
#define SCAN_SETUP_FUNCTION_CPP

#include "../parser/AST.hpp"

#include <queue>

namespace processor {
    void findSetupAuxFunctions (const ast::operation& op);
    void findSetupAuxFunctions (const ast::expression& expr);

    struct setup_expression_visitor : boost::static_visitor<void>
    {
        void operator()(const double& e) const { }
        void operator()(const bool& e) const { }
        void operator()(const std::string& e) const { }
        void operator()(const ast::variable& e) const { }
        void operator()(const ast::unary& e) const { }
        void operator()(const ast::random_statement& e) const {
            findSetupAuxFunctions(e.value);
        }
        void operator()(const ast::expression& e) const {
            findSetupAuxFunctions(e);
        }
        void operator()(const ast::function_call& e) const {
            setup_aux_functions.push_back(e.function_name);
        }
    };

    void findSetupAuxFunctions (const ast::operation& op) {
        boost::apply_visitor(setup_expression_visitor(), op.operand_);
    }

    void findSetupAuxFunctions (const ast::expression& expr) {
        boost::apply_visitor(setup_expression_visitor(), expr.first);
        for (auto it = expr.rest.begin(); it != expr.rest.end(); ++it) {
            findSetupAuxFunctions(*it);
        }
    }

    void scanStatementList (const ast::statement_list& st);

    struct ScanSetup : boost::static_visitor<void> {
        ScanSetup() {}

        void operator()(const ast::variable_declaration& e) const {
            if (e.value)
                findSetupAuxFunctions(*(e.value));
        }
        void operator()(const ast::assignment& e) const {
            findSetupAuxFunctions(e.value);
        }
        void operator()(const ast::single_word_statement& e) const { }
        void operator()(const ast::setxy_statement& e) const { }
        void operator()(const ast::create_agentset& e) const {
            agentset_setup = e;
        }
        void operator()(const ast::move_statement& e) const { }
        void operator()(const ast::hatch_statement& e) const {
            findSetupAuxFunctions(e.quantity);
            scanStatementList(e.body);
        }
        void operator()(const ast::if_statement& e) const { 
            findSetupAuxFunctions(e.condition);
            
            scanStatementList(e.then);
            if (e.else_) {
                scanStatementList(*(e.else_));
            }
        }
        void operator()(const ast::while_statement& e) const {
            findSetupAuxFunctions(e.condition);
            scanStatementList(e.body);
        }
        void operator()(const ast::statement_list& e) const {
            for (auto it = e.begin(); it != e.end(); ++it) {
                boost::apply_visitor(*this, *it);
            }
        }
        void operator()(const ast::function_call& e) const {
            setup_aux_functions.push_back(e.function_name);
            ast::function f = f_list[e.function_name];
            scanStatementList(f.body);
        }
        void operator()(const ast::ask_agentset& e) const { 
            scanStatementList(e.body);
        }
        void operator()(const ast::ask_agent& e) const {
            scanStatementList(e.body);
        }
    };

    void scanStatementList (const ast::statement_list& st) {
        ScanSetup scanSetup;
        for (auto it = st.begin(); it != st.end(); ++it) {
            boost::apply_visitor(scanSetup, *it);
        }
    }

    void scanSetupFunction(ast::main& e) {
        ast::function setup_function = e.functions["setup"];
        scanStatementList(setup_function.body);
    }
}

#endif