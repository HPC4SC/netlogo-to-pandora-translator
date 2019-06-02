#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AST.hpp"
#include "Expression.cpp"
#include "Globals.cpp"
#include "Variable.cpp"
#include "FunctionCall.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It>
    struct statement : qi::grammar<It, ast::statement(), skipper<It> >
    {
        statement()  : statement::base_type(statement_)
        {
            using namespace qi;

            direction.add("lt", ast::left);
            direction.add("left", ast::left);
            direction.add("rt", ast::right);
            direction.add("right", ast::right);
            direction.add("fd", ast::forward);
            direction.add("forward", ast::forward);

            agent.add("turtle", ast::turtle);
            agent.add("patch", ast::patch);
            agent.add("link", ast::link);

            agentset.add("turtles", ast::turtle);
            agentset.add("patches", ast::patch);
            agentset.add("links", ast::link);

            statement_ = 
                        ask_agentset |
                        ask_agent |
                        create_agentset |
                        move_statement |
                        variable_declaration |
                        assignment |
                        function_call_ | 
                        if_statement |
                        while_statement;

            variable_declaration = lexeme["let" >> !(alnum | '_')] > var >> -(expr);

            assignment = lexeme["set" >> !(alnum | '_')] > var > expr;

            if_statement = 
                    lexeme["if" >> !(alnum | '_')] > expr > '[' > +statement_ > ']' 
                        > -( lexeme["else" >> !(alnum | '_')] > +statement_ )
                |   lexeme["ifelse" >> !(alnum | '_')] > expr > '[' > +statement_ > ']' > '[' > +statement_ > ']';


            while_statement = lit("while") > '[' > expr > ']' > '[' > +statement_ > ']';

            ask_agent = lexeme["ask" >> !(alnum | '_')] >> lexeme[agent >> !(alnum | '_')] > expr > '[' > +statement_ > ']';
            ask_agentset = lexeme["ask" >> !(alnum | '_')] >> lexeme[agentset >> !(alnum | '_')] > '[' > +statement_ > ']';

            create_agentset = "create-" > agentset > expr > -('[' > +statement_ > ']');

            move_statement = direction >> expr;
        }

        expression<It> expr;
        variable<It> var;
        function_call<It> function_call_;

        qi::rule<It, ast::statement(), skipper<It> > statement_;
        qi::rule<It, ast::variable_declaration(), skipper<It> > variable_declaration;
        qi::rule<It, ast::assignment(), skipper<It> > assignment;
        qi::rule<It, ast::ask_agent(), skipper<It> > ask_agent;
        qi::rule<It, ast::ask_agentset(), skipper<It> > ask_agentset;
        qi::rule<It, ast::if_statement(), skipper<It> > if_statement;
        qi::rule<It, ast::while_statement(), skipper<It> > while_statement;
        qi::rule<It, ast::create_agentset(), skipper<It> > create_agentset;
        qi::rule<It, ast::move_statement(), skipper<It> > move_statement;
    };
}

#endif