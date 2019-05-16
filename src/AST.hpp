#ifndef AST_HPP
#define AST_HPP

#include <boost/variant/recursive_wrapper.hpp>

namespace ast
{
    struct unary;
    struct expression;
    struct function_call;

    struct identifier
    {
        identifier(std::string const& name = "") : name(name) {}
        std::string name;
    };


    typedef boost::variant<
        std::string,
        double,
        bool,
        identifier,
        boost::recursive_wrapper<unary>,
        boost::recursive_wrapper<expression>,
        boost::recursive_wrapper<function_call>
    > operand;

    enum optoken
    {
        op_plus,
        op_minus,
        op_times,
        op_divide,
        op_positive,
        op_negative,
        op_not,
        op_equal,
        op_not_equal,
        op_less,
        op_less_equal,
        op_greater,
        op_greater_equal,
        op_and,
        op_or
    };

    struct operation {
        optoken operator_;
        operand operand_;
    };

    struct unary {
        optoken operator_;
        operand operand_;
    };

    struct expression
    {
        operand first;
        std::list<operation> rest;
    };

    struct function_call
    {
        identifier function_name;
        std::list<expression> args;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    ast::operation,
    (ast::optoken, operator_)
    (ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::unary,
    (ast::optoken, operator_)
    (ast::operand, operand_)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::expression,
    (ast::operand, first)
    (std::list<ast::operation>, rest)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::function_call,
    (ast::identifier, function_name)
    (std::list<ast::expression>, args)
);

#endif