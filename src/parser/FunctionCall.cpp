#ifndef FUNCTION_CALL_HPP
#define FUNCTION_CALL_HPP

#include "Globals.cpp"
#include "Variable.cpp"
#include "AST.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    template <typename It>
    struct function_call : qi::grammar<It, ast::function_call(), skipper<It> >
    {
        function_call()  : function_call::base_type(start)
        {
            using namespace qi;

            function_name = 
                !lexeme[keywords >> !(alnum | '_')] >> 
                &lexeme[f_args [phx::ref(n_args) = _1] >> !(alnum | '_')] >> 
                raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            start = 
                function_name >> 
                repeat( phx::ref(n_args) )[var];
        }
        
        variable<It> var;

        qi::rule<It, std::string(), skipper<It> > function_name;
        qi::rule<It, ast::function_call(), skipper<It> > start;
    };
}

#endif