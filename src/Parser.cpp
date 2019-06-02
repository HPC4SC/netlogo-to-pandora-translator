#ifndef PARSER_HPP
#define PARSER_HPP

#include "Function.cpp"
#include "Variable.cpp"
#include "AST.hpp"
#include "Skipper.cpp"

#include <boost/spirit/include/qi.hpp>

namespace parser {
    namespace qi = boost::spirit::qi;

    template <typename It>
    struct parser : qi::grammar<It, ast::parser(), skipper<It> >
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

            agents = *( (turtle | patch ) >
                '[' > +variable > ']');

            globals = lit("globals") >
                '[' > +variable > ']';

            start = (agents ^ globals) >>
                +function_;
        }

        function<It> function_;

        qi::rule<It, std::string(), skipper<It> > variable;
        qi::rule<It, std::list<ast::agent>(), skipper<It> > agents;
        qi::rule<It, std::list<std::string>(), skipper<It> > globals;
        qi::rule<It, ast::parser(), skipper<It> > start;

        qi::symbols<char, ast::agent_type> turtle, patch, link, observer;
    };
}

#endif