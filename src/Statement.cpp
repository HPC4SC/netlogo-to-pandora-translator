#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "AST.hpp"
#include "Expression.cpp"
#include "Globals.cpp"
#include "Variable.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    template <typename It>
    struct statement : qi::grammar<It, ast::statement(), skipper<It> >
    {
        statement()  : statement::base_type(statement_)
        {
            using namespace qi;
            
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
                        variable_declaration |
                        assignment |
                        function_call | 
                        if_statement |
                        while_statement;

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

            ask_agent = lexeme["ask" >> !(alnum | '_')] >> lexeme[agent >> !(alnum | '_')] > expr > '[' > +statement_ > ']';
            ask_agentset = lexeme["ask" >> !(alnum | '_')] >> lexeme[agentset >> !(alnum | '_')] > '[' > +statement_ > ']';

            create_agentset = "create-" > agentset > expr > -('[' > +statement_ > ']');
        }

        expression<It> expr;
        variable<It> var;

        qi::rule<It, ast::statement(), skipper<It> > statement_;
        qi::rule<It, ast::variable_declaration(), skipper<It> > variable_declaration;
        qi::rule<It, ast::assignment(), skipper<It> > assignment;
        qi::rule<It, ast::function_call(), skipper<It> > function_call;
        qi::rule<It, ast::ask_agent(), skipper<It> > ask_agent;
        qi::rule<It, ast::ask_agentset(), skipper<It> > ask_agentset;
        qi::rule<It, ast::if_statement(), skipper<It> > if_statement;
        qi::rule<It, ast::while_statement(), skipper<It> > while_statement;
        qi::rule<It, std::string(), skipper<It> > function_name;
        qi::rule<It, ast::create_agentset(), skipper<It> > create_agentset;
        
        qi::symbols<char, ast::agent_type> agent, agentset;
    };
}

#endif