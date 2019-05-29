#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "Globals.cpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It, typename Skipper = qi::space_type>
    struct variable : qi::grammar<It, std::string(), Skipper>
    {
        variable()  : variable::base_type(start)
        {
            using namespace qi;

            start =
                !lexeme[keywords >> !(alnum | '_' | '-' | '?' | '%')] >> 
                !lexeme[f_args >> !(alnum | '_' | '-' | '?' | '%')] >>
                raw[lexeme[*(alnum | '_' | '-' | '?' | '%')]];
        }

        qi::rule<It, std::string(), Skipper > start;
    };
}

#endif