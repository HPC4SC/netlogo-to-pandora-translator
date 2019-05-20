#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AST.hpp"
#include "Expression.cpp"

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
                        //compound_statement | 
                        if_statement |
                        while_statement |
                        return_statement;

            identifier = !expr.keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

            variable_declaration = lexeme["let" >> !(alnum | '_')] > identifier > -(expr);

            assignment = lexeme["set" >> !(alnum | '_')] > identifier > expr;

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
        qi::rule<It, ast::if_statement(), Skipper> if_statement;
        qi::rule<It, ast::while_statement(), Skipper> while_statement;
        qi::rule<It, ast::return_statement(), Skipper> return_statement;
        qi::rule<It, std::string(), Skipper> identifier;
    };
}

#endif