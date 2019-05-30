#ifndef SKIPPER_HPP
#define SKIPPER_HPP

#include <boost/spirit/include/qi.hpp>

namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    struct skipper : qi::grammar<Iterator>
    {
        skipper() : skipper::base_type(start)
        {
            start = ascii::space | ";" >> *(qi::char_ - '\n') >> qi::eol;
        }

        qi::rule<Iterator> start;
    };
}

#endif