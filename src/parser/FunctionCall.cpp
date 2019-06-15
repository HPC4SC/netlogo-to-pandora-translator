#ifndef FUNCTION_CALL_HPP
#define FUNCTION_CALL_HPP

#include "../Globals.cpp"
#include "Variable.cpp"
#include "AST.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    std::string check (std::string name) {
        n_args = f_args.at(name);
        return name;
    }

    ast::function_call store (std::string name, std::vector<ast::variable> args) {
        std::list<ast::variable> myList(args.begin(), args.end());
        ast::function_call ret = { name, myList };
        return ret;
    }

    template <typename It>
    struct function_call : qi::grammar<It, ast::function_call(), skipper<It> >
    {
        function_call()  : function_call::base_type(start)
        {
            using namespace qi;

            function_name = 
                (
                    &f_args >> 
                    raw[lexeme[*(alnum | '_' | '-' | '?' | '%')]]
                );

            start = (
                function_name [ _1 = phx::bind(&check, _1) ] >> 
                repeat( phx::ref(n_args) )[var]
            ) [ _val = phx::bind(&store, _1, _2) ]
            ;


        }
        
        variable<It> var;

        qi::rule<It, std::string(), skipper<It> > function_name;
        qi::rule<It, ast::function_call(), skipper<It> > start;
    };
}

#endif