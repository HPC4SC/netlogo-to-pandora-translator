#ifndef AGENTSET_EXPRESSION_HPP
#define AGENTSET_EXPRESSION_HPP

#include "Expression.cpp"
#include "FunctionCall.cpp"
#include "Variable.cpp"
#include "../Globals.cpp"
#include "AST.hpp"

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    template <typename It>
    struct agentset_expression : qi::grammar<It, ast::agentset_expression(), skipper<It> >
    {
        agentset_expression()  : agentset_expression::base_type(agentset_expr)
        {
            using namespace qi;

            agentset.add("turtles", ast::turtle);
            agentset.add("patches", ast::patch);
            agentset.add("links", ast::link);

            agentset_expr = 
                        agentset_with_reporter |
                        n_of_agentset |
                        other_agentset |
                        agentset_here |
                        base_agentset;

            agentset_with_reporter = (n_of_agentset | base_agentset) >> lit("with") > '[' > expr > ']';

            n_of_agentset = "n-of" > expr > agentset_expr;

            other_agentset = "other" > agentset_expr;

            agentset_here = agentset >> "-here";

            base_agentset = agentset;
        }

        expression<It> expr;
        variable<It> var;
        function_call<It> function_call_;

        qi::rule<It, ast::agentset_expression(), skipper<It> > agentset_expr;

        qi::rule<It, ast::n_of_agentset(), skipper<It> > n_of_agentset;
        qi::rule<It, ast::other_agentset(), skipper<It> > other_agentset;
        qi::rule<It, ast::base_agentset(), skipper<It> > base_agentset;
        qi::rule<It, ast::agentset_here(), skipper<It> > agentset_here;
        qi::rule<It, ast::agentset_with_reporter(), skipper<It> > agentset_with_reporter;
    };
}

#endif