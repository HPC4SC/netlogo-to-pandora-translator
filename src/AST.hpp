#ifndef AST_HPP
#define AST_HPP

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/tuple/tuple.hpp>

namespace ast
{
    struct unary;
    struct expression;
    struct function_call;

    enum agent_type {
        turtle,
        patch,
        link,
        observer
    };

    struct agent
    {
        agent_type type;
        std::list<std::string> attributes;
    };

    typedef boost::variant<
        std::string,
        double,
        bool,
        std::string,
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
    struct ask;

    typedef boost::variant<
            variable_declaration,
            assignment,
            boost::recursive_wrapper<function_call>,
            boost::recursive_wrapper<ask>,
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
        statement_list then;
        boost::optional<statement_list> else_;
    };

    struct while_statement
    {
        expression condition;
        statement_list body;
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

    struct ask
    {
        agent_type type;
        statement_list body;
    };
    
    typedef std::list<agent> agent_list;
    typedef std::list<std::string> global_list;

    struct configuration
    {
        agent_list agents;
        global_list globals;
    };

    struct parser
    {
        configuration config;
        std::list<function> functions;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    ast::agent,
    (ast::agent_type, type)
    (std::list<std::string>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::configuration,
    (ast::agent_list, agents)
    (ast::global_list, globals)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::parser,
    (ast::configuration, config)
    (std::list<ast::function>, functions)
)

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
    (ast::statement_list, then)
    (boost::optional<ast::statement_list>, else_)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::while_statement,
    (ast::expression, condition)
    (ast::statement_list, body)
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

BOOST_FUSION_ADAPT_STRUCT(
    ast::ask,
    (ast::agent_type, type)
    (ast::statement_list, body)
);

#endif