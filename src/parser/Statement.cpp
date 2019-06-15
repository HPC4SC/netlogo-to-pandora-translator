#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AgentsetExpression.cpp"
#include "Expression.cpp"
#include "FunctionCall.cpp"
#include "Variable.cpp"
#include "../Globals.cpp"
#include "AST.hpp"

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

            single_word_callback.add("die", ast::die);
            single_word_callback.add("clear-all", ast::clear_all);
            single_word_callback.add("tick", ast::tick);
            single_word_callback.add("reset-ticks", ast::reset_ticks);

            statement_ = 
                        ask_agentset |
                        ask_agent |
                        create_agentset |
                        move_statement |
                        setxy_statement |
                        single_word_statement |
                        variable_declaration |
                        assignment |
                        function_call_ | 
                        if_statement |
                        while_statement;

            variable_declaration = lit("let") > var >> -(expr);

            assignment = lit("set") > var > expr;

            if_statement = 
                    "if" >> expr >> '[' > +statement_ > ']' > -( "else" > +statement_ )
                |   "ifelse" >> expr >> '[' > +statement_ > ']' > '[' > +statement_ > ']';


            while_statement = lit("while") > '[' > expr > ']' > '[' > +statement_ > ']';

            ask_agent = "ask" >> agent >> expr >> '[' >> +statement_ >> ']';
            ask_agentset = "ask" >> agentset_expression_ >> '[' >> +statement_ >> ']';

            create_agentset = "create-" > agentset > expr > -('[' > +statement_ > ']');

            move_statement = direction >> expr;

            setxy_statement = "setxy" >> expr >> expr;

            single_word_statement = single_word_callback;
        }

        expression<It> expr;
        variable<It> var;
        function_call<It> function_call_;
        agentset_expression<It> agentset_expression_;

        qi::rule<It, ast::statement(), skipper<It> > statement_;
        qi::rule<It, ast::variable_declaration(), skipper<It> > variable_declaration;
        qi::rule<It, ast::assignment(), skipper<It> > assignment;
        qi::rule<It, ast::ask_agent(), skipper<It> > ask_agent;
        qi::rule<It, ast::ask_agentset(), skipper<It> > ask_agentset;
        qi::rule<It, ast::if_statement(), skipper<It> > if_statement;
        qi::rule<It, ast::while_statement(), skipper<It> > while_statement;
        qi::rule<It, ast::create_agentset(), skipper<It> > create_agentset;
        qi::rule<It, ast::move_statement(), skipper<It> > move_statement;
        qi::rule<It, ast::setxy_statement(), skipper<It> > setxy_statement;
        qi::rule<It, ast::single_word_statement(), skipper<It> > single_word_statement;
    };
}

#endif