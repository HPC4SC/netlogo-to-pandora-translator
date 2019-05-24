#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AST.hpp"
#include "Expression.cpp"
#include "Globals.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It, typename Skipper = qi::space_type>
    struct statement : qi::grammar<It, ast::statement_list(), Skipper>
    {
        statement()  : statement::base_type(statement_list)
        {
            using namespace qi;

            statement_list = +statement_;

            statement_ = variable_declaration |
                        assignment |
                        function_call | 
                        if_statement |
                        while_statement |
                        return_statement;

            identifier = !keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            variable_declaration = lexeme["let" >> !(alnum | '_')] > identifier > -(expr);

            assignment = lexeme["set" >> !(alnum | '_')] > identifier > expr;

            function_call = 
                function_name >> 
                repeat( phx::ref(n_args) )[identifier];

            function_name = 
                !lexeme[keywords >> !(alnum | '_')] >> 
                &lexeme[f_args [phx::ref(n_args) = _1] >> !(alnum | '_')] >> 
                raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            if_statement = 
                    lexeme["if" >> !(alnum | '_')] > expr > '[' > statement_ > ']' 
                        > -( lexeme["else" >> !(alnum | '_')] > statement_ )
                |   lexeme["ifelse" >> !(alnum | '_')] > expr > '[' > statement_ > ']' > '[' > statement_ > ']';


            while_statement = lit("while") > '[' > expr > ']' > '[' > statement_ > ']';

            return_statement = lexeme["report" >> !(alnum | '_')] > expr;
        }

        expression<It, Skipper> expr;

        qi::rule<It, ast::statement_list(), Skipper> statement_list;
        qi::rule<It, ast::statement(), Skipper > statement_;
        qi::rule<It, ast::variable_declaration(), Skipper> variable_declaration;
        qi::rule<It, ast::assignment(), Skipper> assignment;
        qi::rule<It, ast::function_call(), Skipper> function_call;
        qi::rule<It, ast::if_statement(), Skipper> if_statement;
        qi::rule<It, ast::while_statement(), Skipper> while_statement;
        qi::rule<It, ast::return_statement(), Skipper> return_statement;
        qi::rule<It, std::string(), Skipper> identifier;
        qi::rule<It, std::string(), Skipper> function_name;
    };
}

#endif