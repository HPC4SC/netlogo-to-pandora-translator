#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AST.hpp"
#include "Expression.cpp"
#include "Globals.cpp"
#include "Variable.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It, typename Skipper = qi::space_type>
    struct statement : qi::grammar<It, ast::statement(), Skipper>
    {
        statement()  : statement::base_type(statement_)
        {
            using namespace qi;
            
            turtle.add("turtles", ast::turtle);
            patch.add("patches", ast::patch);
            link.add("links", ast::link);

            statement_ = 
                        ask |
                        variable_declaration |
                        assignment |
                        function_call | 
                        if_statement |
                        while_statement |
                        return_statement;

            variable_declaration = lexeme["let" >> !(alnum | '_')] > var >> -(expr);

            assignment = lexeme["set" >> !(alnum | '_')] > var > expr;

            function_call = 
                function_name >> 
                repeat( phx::ref(n_args) )[var];

            function_name = 
                !lexeme[keywords >> !(alnum | '_')] >> 
                &lexeme[f_args [phx::ref(n_args) = _1] >> !(alnum | '_')] >> 
                raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            if_statement = 
                    lexeme["if" >> !(alnum | '_')] > expr > '[' > +statement_ > ']' 
                        > -( lexeme["else" >> !(alnum | '_')] > +statement_ )
                |   lexeme["ifelse" >> !(alnum | '_')] > expr > '[' > +statement_ > ']' > '[' > +statement_ > ']';


            while_statement = lit("while") > '[' > expr > ']' > '[' > +statement_ > ']';

            return_statement = lexeme["report" >> !(alnum | '_')] > expr;

            ask = lexeme["ask" >> !(alnum | '_')] > (turtle | patch | link) > '[' > +statement_ > ']';
        }

        expression<It, Skipper> expr;
        variable<It> var;

        qi::rule<It, ast::statement(), Skipper > statement_;
        qi::rule<It, ast::variable_declaration(), Skipper> variable_declaration;
        qi::rule<It, ast::assignment(), Skipper> assignment;
        qi::rule<It, ast::function_call(), Skipper> function_call;
        qi::rule<It, ast::ask(), Skipper> ask;
        qi::rule<It, ast::if_statement(), Skipper> if_statement;
        qi::rule<It, ast::while_statement(), Skipper> while_statement;
        qi::rule<It, ast::return_statement(), Skipper> return_statement;
        qi::rule<It, std::string(), Skipper> function_name;

        
        qi::symbols<char, ast::agent_type> turtle, patch, link, observer;
    };
}

#endif