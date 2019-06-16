#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include "parser/AST.hpp"

#include <boost/spirit/include/qi.hpp>

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
}

ast::function_list f_list;
std::string context = "";
std::list<std::string> agentAttributes;

#endif