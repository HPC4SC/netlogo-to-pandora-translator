#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "AST.hpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It, typename Skipper = qi::space_type>
    struct function : qi::grammar<It, ast::function(), Skipper>
    {
        function();

        
    };
}

#endif