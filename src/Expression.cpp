#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>

#include "AST.hpp"

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;

#ifndef EXPRESSION_H
#define EXPRESSION_H

namespace parser {

    /********* GRAMMAR *********/

    template <typename It, typename Skipper = qi::space_type>
        struct expression : qi::grammar<It, ast::expression(), Skipper>
    {
        expression() : expression::base_type(expr)
        {
            using namespace qi;

            logical_or_op.add
                ("||", ast::op_or)
                ;

            logical_and_op.add
                ("&&", ast::op_and)
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
                ("!", ast::op_not)
                ;

            expr = logical_or_expr.alias();

            logical_or_expr     %= logical_and_expr >> *(logical_or_op > logical_or_expr);

            logical_and_expr    %= equality_expr >> *(logical_and_op >> logical_and_expr);

            equality_expr       %= relational_expr >> *(equality_op >> equality_expr);

            relational_expr     %= additive_expr >> *(relational_op >> relational_expr);

            additive_expr       %= multiplicative_expr >> *(additive_op >> additive_expr);

            multiplicative_expr %= unary_expr >> *(multiplicative_op >> multiplicative_expr);

            unary_expr          %= unary_op > primary_expr | primary_expr;

            primary_expr %= var_    |
                            double_ |
                            bool_   |
                            '(' >> expr >> ')';

            var_ = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

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


        qi::rule<It, std::string(), Skipper> var_;
        qi::rule<It, ast::expression(), Skipper> expr, 
            equality_expr, relational_expr,
            logical_or_expr, logical_and_expr,
            additive_expr, multiplicative_expr
            ;
        qi::rule<It, ast::operand(), Skipper> unary_expr, primary_expr;

        qi::symbols<char, ast::optoken>
            logical_or_op, logical_and_op,
            equality_op, relational_op,
            additive_op, multiplicative_op, unary_op
            ;
    };
}
#endif