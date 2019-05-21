#ifndef AST_HPP
#define AST_HPP

#include <boost/variant/recursive_wrapper.hpp>

namespace ast
{
    struct unary;
    struct expression;
    struct function_call;



    typedef boost::variant<
        std::string,
        double,
        bool,
        std::string,
        boost::recursive_wrapper<unary>,
        boost::recursive_wrapper<expression>
        //boost::recursive_wrapper<function_call>
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
        std::string function_name;
        std::list<std::string> args;
    };

    struct assignment
    {
        std::string lhs;
        expression rhs;
    };

    struct variable_declaration
    {
        std::string lhs;
        boost::optional<expression> rhs;
    };

    struct if_statement;
    struct while_statement;
    struct statement_list;
    struct return_statement;

    typedef boost::variant<
            variable_declaration,
            assignment,
            //boost::recursive_wrapper<function_call>,
            boost::recursive_wrapper<if_statement>, 
            boost::recursive_wrapper<while_statement>, 
            boost::recursive_wrapper<return_statement>, 
            boost::recursive_wrapper<statement_list>
        >
    statement;

    struct statement_list : std::list<statement> {};

    struct if_statement
    {
        expression condition;
        statement then;
        boost::optional<statement> else_;
    };

    struct while_statement
    {
        expression condition;
        statement body;
    };

    struct return_statement
    {
        boost::optional<expression> expr;
    };

    struct function
    {
        std::string return_type;
        std::string function_name;
        std::list<std::string> args;
        statement_list body;
    };

    typedef std::list<function> function_list;
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
    (std::string, function_name)
    (std::list<std::string>, args)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::variable_declaration,
    (std::string, lhs)
    (boost::optional<ast::expression>, rhs)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::assignment,
    (std::string, lhs)
    (ast::expression, rhs)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::if_statement,
    (ast::expression, condition)
    (ast::statement, then)
    (boost::optional<ast::statement>, else_)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::while_statement,
    (ast::expression, condition)
    (ast::statement, body)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::return_statement,
    (boost::optional<ast::expression>, expr)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::function,
    (std::string, return_type)
    (std::string, function_name)
    (std::list<std::string>, args)
    (ast::statement_list, body)
);

#endif