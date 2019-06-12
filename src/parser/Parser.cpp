#ifndef PARSER_HPP
#define PARSER_HPP

#include "Function.cpp"
#include "Variable.cpp"
#include "AST.hpp"
#include "Skipper.cpp"

#include <boost/spirit/include/qi.hpp>
#include <map>

namespace parser {
    namespace qi = boost::spirit::qi;

    ast::function_list generate_function_map (std::vector<ast::function, std::allocator<ast::function> > functions) {
        ast::function_list ret;
        for (auto it = functions.begin(); it != functions.end(); ++it) {
            ret[it->function_name] = *it;
        }
        return ret;
    }

    template <typename It>
    struct parser : qi::grammar<It, ast::main(), skipper<It> >
    {
        parser()  : parser::base_type(start)
        {
            using namespace qi;

            turtle.add("turtles-own", ast::turtle);
            patch.add("patches-own", ast::patch);
            link.add("link-own", ast::link);
            observer.add("observer-own", ast::observer);

            // TODO: Check why using Variable.cpp here slows down a lot the parser
            variable = raw[lexeme[(alpha | '_' | '%') >> *(alnum | '_' | '-' | '?' | '%')]];

            agents = *( (turtle | patch ) >>
                '[' > +variable > ']');

            globals = lit("globals") >>
                '[' > +variable > ']';

            functions = (+function_) [ _val = phx::bind(&generate_function_map, _1) ];

            start = (agents ^ globals) >> functions;
        }

        function<It> function_;

        qi::rule<It, std::string(), skipper<It> > variable;
        qi::rule<It, std::list<ast::agent>(), skipper<It> > agents;
        qi::rule<It, std::list<std::string>(), skipper<It> > globals;
        qi::rule<It, ast::function_list(), skipper<It> > functions;
        qi::rule<It, ast::main(), skipper<It> > start;

        qi::symbols<char, ast::agent_type> turtle, patch, link, observer;
    };
}

#endif