#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include <boost/spirit/include/qi.hpp>

namespace parser {
    namespace qi = boost::spirit::qi;

    qi::symbols<char, int> f_args;
}

#endif