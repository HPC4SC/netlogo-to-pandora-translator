#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include <boost/spirit/include/qi.hpp>

namespace parser {
    namespace qi = boost::spirit::qi;

    int n_args;
    qi::symbols<char, int> f_args;
    qi::symbols<char> keywords;
    qi::symbols<char, ast::agent_type> agent, agentset;

    void init_globals () {
        agent.add("turtle", ast::turtle);
        agent.add("patch", ast::patch);
        agent.add("link", ast::link);

        agentset.add("turtles", ast::turtle);
        agentset.add("patches", ast::patch);
        agentset.add("links", ast::link);
    }
}

#endif