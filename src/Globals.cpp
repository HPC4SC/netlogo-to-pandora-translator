#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include "parser/AST.hpp"

#include <boost/spirit/include/qi.hpp>

std::string removeInvalidChars (std::string name) {
    name.erase(std::remove(name.begin(), name.end(), '-'), name.end());
    name.erase(std::remove(name.begin(), name.end(), '?'), name.end());
    name.erase(std::remove(name.begin(), name.end(), '%'), name.end());
    return name;
}

namespace parser {
    namespace qi = boost::spirit::qi;

    int n_args;
    qi::symbols<char, int> f_args;
    qi::symbols<char> keywords;
    qi::symbols<char, ast::agent_type> agent, agentset;
    qi::symbols<char, ast::move_dir> direction;
    qi::symbols<char, ast::callback_keyword> single_word_callback;
}

namespace processor {
    int actionId = 0;
    std::map<std::string, ast::ask_agentset> agent_actions;
    std::list<std::string> agent_aux_functions;
    std::list<std::string> setup_aux_functions;
    ast::create_agentset agentset_setup;
}

ast::function_list f_list;
std::string context = "";
std::list<std::string> agentAttributes;

#endif