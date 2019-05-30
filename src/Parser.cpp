#ifndef PARSER_HPP
#define PARSER_HPP

#include "Statement.cpp"

namespace parser {

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