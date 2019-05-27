#ifndef PARSER_HPP
#define PARSER_HPP

#include "Statement.cpp"

namespace parser {

    template <typename It, typename Skipper = qi::space_type>
    struct parser : qi::grammar<It, ast::parser(), Skipper>
    {
        parser()  : parser::base_type(start)
        {
            using namespace qi;

            turtle.add("turtles-own", ast::turtle);
            turtle.add("patch-own", ast::turtle);
            turtle.add("link-own", ast::turtle);
            turtle.add("observer-own", ast::turtle);

            variable = !keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-' | '?')]];

            agent = turtle >
                '[' > +variable > ']';

            globals = lit("globals") >
                '[' > +variable > ']';

            start = *agent ^ globals >> 
                +function_;
        }

        function<It> function_;

        qi::rule<It, std::string(), Skipper > variable;
        qi::rule<It, ast::agent(), Skipper > agent;
        qi::rule<It, std::list<std::string>(), Skipper > globals;
        qi::rule<It, ast::parser(), Skipper > start;

        qi::symbols<char, ast::agent_type> turtle, patch, link, observer;
    };
}

#endif