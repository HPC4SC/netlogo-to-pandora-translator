#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Statement.cpp"
#include "Expression.cpp"
#include "../Globals.cpp"
#include "AST.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    ast::function store_function (std::string name, ast::function_args args, ast::statement_list body, ast::return_statement return_) {
        int num_args = args ? (*args).size() : 0;
        f_args.add(name, num_args);
        
        ast::function ret_value = { name, args, body, return_ };
        return ret_value;
    }

    template <typename It>
    struct function : qi::grammar<It, ast::function(), skipper<It> >
    {
        function()  : function::base_type(function_)
        {
            using namespace qi;

            name = !(keywords >> ' ') >> !(f_args >> ' ') >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-' | '%')]];

            identifier = name;
            argument_list = -('[' > *identifier > ']');

            return_statement = -(lit("report") > expr);

            body_ = +statement_;

            function_ = (
                    (lit("to-report") | lit("to") )
                >   identifier 
                >   argument_list
                >   body_
                >   return_statement
                >   lexeme[string("end") >> !(alnum | '_')]
            ) [ _val = phx::bind(&store_function, _1, _2, _3, _4) ];
        }

        expression<It> expr;
        statement<It> statement_;

        qi::rule<It, std::string(), skipper<It> > name;
        qi::rule<It, std::string(), skipper<It> > identifier;
        qi::rule<It, ast::function_args(), skipper<It> > argument_list;
        qi::rule<It, ast::return_statement(), skipper<It> > return_statement;
        qi::rule<It, ast::statement_list(), skipper<It> > body_;
        qi::rule<It, ast::function(), skipper<It> > function_, command, reporter;
    };
}

#endif