#ifndef GLOBALS_CPP
#define GLOBALS_CPP

#include <boost/spirit/include/qi.hpp>

namespace parser {
    namespace qi = boost::spirit::qi;

    int n_args;
    qi::symbols<char, int> f_args;
    
    qi::symbols<char> keywords;
}

#endif