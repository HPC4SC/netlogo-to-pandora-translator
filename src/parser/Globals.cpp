#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include "AST.hpp"

#include <boost/spirit/include/qi.hpp>

namespace parser {
    namespace qi = boost::spirit::qi;

    int n_args;
    qi::symbols<char, int> f_args;
    qi::symbols<char> keywords;
    qi::symbols<char, ast::agent_type> agent, agentset;
    qi::symbols<char, ast::move_dir> direction;
    qi::symbols<char, ast::callback_keyword> single_word_callback;

    ast::function_list f_list;
}

#endif