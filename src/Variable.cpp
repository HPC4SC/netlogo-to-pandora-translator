#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "Globals.cpp"
#include "Skipper.cpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It>
    struct variable : qi::grammar<It, std::string(), skipper<It> >
    {
        variable()  : variable::base_type(start)
        {
            using namespace qi;

            start =
                !lexeme[keywords >> !(alnum | '_' | '-' | '?' | '%')] >> 
                !lexeme[f_args >> !(alnum | '_' | '-' | '?' | '%')] >>
                raw[lexeme[*(alnum | '_' | '-' | '?' | '%')]];
        }

        qi::rule<It, std::string(), skipper<It> > start;
    };
}

#endif