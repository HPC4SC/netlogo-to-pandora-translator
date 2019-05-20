#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Statement.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It, typename Skipper = qi::space_type>
    struct function : qi::grammar<It, ast::function(), Skipper>
    {
        function()  : function::base_type(start)
        {
            using namespace qi;

            name = !body.expr.keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            identifier = name;
            argument_list = +identifier;

            start = lexeme[(string("to-report") | string("to"))
                        >> !(alnum | '_')]  // make sure we have whole words
                >   identifier
                >   -('[' > argument_list > ']') 
                >   body
                >   lexeme[string("end") >> !(alnum | '_')]
                ;
        }

        statement<It> body;
        qi::rule<It, std::string(), Skipper > name;
        qi::rule<It, ast::identifier(), Skipper > identifier;
        qi::rule<It, std::list<ast::identifier>(), Skipper > argument_list;
        qi::rule<It, ast::function(), Skipper > start;
    };
}

#endif