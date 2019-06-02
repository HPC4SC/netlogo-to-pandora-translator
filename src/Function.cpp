#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Statement.cpp"
#include "Expression.cpp"
#include "Globals.cpp"
#include "AST.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    void store_function (std::string name, std::list<std::string> args) {
        f_args.add(name, args.size());
        std::cout << name << " " <<  args.size() << std::endl;
    }

    template <typename It>
    struct function : qi::grammar<It, ast::function(), skipper<It> >
    {
        function()  : function::base_type(function_)
        {
            using namespace qi;

            name = !keywords >> !lexeme[f_args >> !(alnum | '_')] >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-' | '%')]];

            identifier = name;
            argument_list = *identifier;

            return_statement = lexeme["report" >> !(alnum | '_')] > expr;

            command = (
                    lexeme[string("to") >> !(alnum | '_')]
                >   identifier 
                >   ('[' > argument_list > ']')
                >   +statement_
                >   lexeme[string("end") >> !(alnum | '_')]
            ) [ phx::bind(&store_function, _2, _3) ];

            reporter = (
                    lexeme[string("to-report") >> !(alnum | '_')]
                >   identifier 
                >   ('[' > argument_list > ']')
                >   +statement_
                >   return_statement
                >   lexeme[string("end") >> !(alnum | '_')]
            ) [ phx::bind(&store_function, _2, _3) ];

            function_ = command | reporter;
        }

        expression<It> expr;
        statement<It> statement_;

        qi::rule<It, std::string(), skipper<It> > name;
        qi::rule<It, std::string(), skipper<It> > identifier;
        qi::rule<It, std::list<std::string>(), skipper<It> > argument_list;
        qi::rule<It, ast::return_statement(), skipper<It> > return_statement;
        qi::rule<It, ast::function(), skipper<It> > function_, command, reporter;
    };
}

#endif