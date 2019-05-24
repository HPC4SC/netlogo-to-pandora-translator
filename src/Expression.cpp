#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "AST.hpp"
#include "Globals.cpp"

namespace parser {

    namespace qi = boost::spirit::qi;

    /********* GRAMMAR *********/

    template <typename It, typename Skipper = qi::space_type>
        struct expression : qi::grammar<It, ast::expression(), Skipper>
    {
        expression() : expression::base_type(expr)
        {
            using namespace qi;

            logical_or_op.add
                ("or", ast::op_or)
                ;

            logical_and_op.add
                ("and", ast::op_and)
                ;

            equality_op.add
                ("==", ast::op_equal)
                ("!=", ast::op_not_equal)
                ;

            relational_op.add
                ("<", ast::op_less)
                ("<=", ast::op_less_equal)
                (">", ast::op_greater)
                (">=", ast::op_greater_equal)
                ;

            additive_op.add
                ("+", ast::op_plus)
                ("-", ast::op_minus)
                ;

            multiplicative_op.add
                ("*", ast::op_times)
                ("/", ast::op_divide)
                ;

            unary_op.add
                ("not", ast::op_not)
                ;

            keywords.add
                ("true")
                ("false")
                ("if")
                ("else")
                ("while")
                ("int")
                ("void")
                ("return")
                ;


            expr = logical_or_expr.alias();

            logical_or_expr     %= logical_and_expr >> *(logical_or_op > logical_or_expr);

            logical_and_expr    %= equality_expr >> *(logical_and_op > logical_and_expr);

            equality_expr       %= relational_expr >> *(equality_op > equality_expr);

            relational_expr     %= additive_expr >> *(relational_op > relational_expr);

            additive_expr       %= multiplicative_expr >> *(additive_op > additive_expr);

            multiplicative_expr %= unary_expr >> *(multiplicative_op > multiplicative_expr);

            unary_expr          %= unary_op > primary_expr | primary_expr;

            primary_expr %= double_ |
                            //function_call |
                            identifier |
                            bool_   |
                            '(' >> expr >> ')';

            //function_call = identifier >> *identifier;

            identifier = !lexeme[keywords >> !(alnum | '_')] 
                >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

            BOOST_SPIRIT_DEBUG_NODES(
                (expr)
                (logical_or_expr)
                (logical_and_expr)
                (equality_expr)
                (relational_expr)
                (additive_expr)
                (multiplicative_expr)
                (unary_expr)
                (primary_expr)
            );
        }


        qi::rule<It, ast::operand(), Skipper> unary_expr, primary_expr;
        //qi::rule<It, ast::function_call(), Skipper> function_call;
        qi::rule<It, std::string(), Skipper> identifier;
        qi::rule<It, ast::expression(), Skipper> expr, 
            equality_expr, relational_expr,
            logical_or_expr, logical_and_expr,
            additive_expr, multiplicative_expr
            ;

        qi::symbols<char, ast::optoken>
            logical_or_op, logical_and_op,
            equality_op, relational_op,
            additive_op, multiplicative_op, unary_op
            ;

        qi::symbols<char> keywords;
    };
}
#endif