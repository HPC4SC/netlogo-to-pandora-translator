#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Statement.cpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;
    
    qi::symbols<char> f_args;

    template <typename It, typename Skipper = qi::space_type>
    struct function : qi::grammar<It, ast::function_list(), Skipper>
    {
        function()  : function::base_type(start)
        {
            using namespace qi;

            name = !body.expr.keywords >> !lexeme[f_args >> !(alnum | '_')] >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            identifier = name;
            argument_list = +identifier;

            start = +function_;

            function_ = (lexeme[(string("to-report") | string("to"))
                        >> !(alnum | '_')]  // make sure we have whole words
                >   identifier 
                >   -('[' > argument_list > ']') 
                >   body
                >   lexeme[string("end") >> !(alnum | '_')])
                [ phx::bind(f_args.add, _2) ]
                ;
        }

        statement<It> body;
        qi::rule<It, std::string(), Skipper > name;
        qi::rule<It, std::string(), Skipper > identifier;
        qi::rule<It, std::list<std::string>(), Skipper > argument_list;
        qi::rule<It, ast::function(), Skipper > function_;
        qi::rule<It, ast::function_list(), Skipper > start;
    };
}

#endif