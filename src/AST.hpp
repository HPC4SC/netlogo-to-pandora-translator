#ifndef AST_HPP
#define AST_HPP

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <list>

namespace ast
{
    struct unary;
    struct expression;
    struct function_call;
    struct random_statement;
    struct count_agentset;

    enum agent_type {
        turtle,
        patch,
        link,
        observer
    };

    enum move_dir {
        left,
        right,
        forward
    };

    enum callback_keyword {
        die,
        clear_all,
        tick,
        reset_ticks
    };

    struct agent
    {
        agent_type type;
        std::list<std::string> attributes;
    };

    typedef boost::variant<
        double,
        bool,
        std::string,
        boost::recursive_wrapper<unary>,
        boost::recursive_wrapper<expression>,
        boost::recursive_wrapper<function_call>,
        boost::recursive_wrapper<random_statement>,
        boost::recursive_wrapper<count_agentset>
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
    struct ask_agentset;
    struct ask_agent;
    struct create_agentset;
    struct move_statement;
    struct setxy_statement;
    struct single_word_statement;

    typedef boost::variant<
            variable_declaration,
            assignment,
            single_word_statement,
            setxy_statement,
            boost::recursive_wrapper<function_call>,
            boost::recursive_wrapper<ask_agentset>,
            boost::recursive_wrapper<ask_agent>,
            boost::recursive_wrapper<create_agentset>,
            boost::recursive_wrapper<move_statement>,
            boost::recursive_wrapper<if_statement>,
            boost::recursive_wrapper<while_statement>,
            boost::recursive_wrapper<statement_list>
        >
    statement;






    // Agentset Expression

    struct base_agentset;
    struct agentset_here;
    struct other_agentset;
    struct n_of_agentset;
    struct agentset_with_reporter;

    typedef boost::variant<
            base_agentset,
            agentset_here,
            boost::recursive_wrapper<n_of_agentset>,
            boost::recursive_wrapper<agentset_with_reporter>,
            boost::recursive_wrapper<other_agentset>
        >
    agentset_expression;

    struct base_agentset {
        agent_type type;
    };

    struct agentset_here {
        agent_type type;
    };

    struct n_of_agentset {
        expression expr;
        agentset_expression agentset_expr;
    };

    struct agentset_with_reporter {
        agentset_expression agentset_expr;
        expression expr;
    };

    struct other_agentset {
        agentset_expression agentset_expr;
    };







    struct count_agentset {
        agentset_expression agentset_expr;
    };



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
        std::string function_name;
        std::list<std::string> args;
        statement_list body;
        return_statement return_;
    };

    struct ask_agentset
    {
        agentset_expression type;
        statement_list body;
    };

    struct ask_agent
    {
        agent_type type;
        expression id;
        statement_list body;
    };
    
    typedef std::list<agent> agent_list;
    typedef std::list<std::string> global_list;

    struct create_agentset {
        agent_type type;
        expression quantity;
        statement_list body;
    };

    struct random_statement {
        expression value;
    };

    struct move_statement {
        move_dir direction;
        expression quantity;
    };

    struct setxy_statement {
        expression x;
        expression y;
    };

    struct single_word_statement {
        callback_keyword keyword;
    };

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
    ast::n_of_agentset,
    (ast::expression, expr)
    (ast::agentset_expression, agentset_expr)
) 

BOOST_FUSION_ADAPT_STRUCT(
    ast::base_agentset,
    (ast::agent_type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::agentset_here,
    (ast::agent_type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::agentset_with_reporter,
    (ast::agentset_expression, agentset_expr)
    (ast::expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::other_agentset,
    (ast::agentset_expression, agentset_expr)
) 



BOOST_FUSION_ADAPT_STRUCT(
    ast::count_agentset,
    (ast::agentset_expression, agentset_expr)
) 


BOOST_FUSION_ADAPT_STRUCT(
    ast::setxy_statement,
    (ast::expression, x)
    (ast::expression, y)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::random_statement,
    (ast::expression, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::single_word_statement,
    (ast::callback_keyword, keyword)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::move_statement,
    (ast::move_dir, direction)
    (ast::expression, quantity)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::create_agentset,
    (ast::agent_type, type)
    (ast::expression, quantity)
    (ast::statement_list, body)
)

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
    ast::ask_agentset,
    (ast::agentset_expression, type)
    (ast::statement_list, body)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::ask_agent,
    (ast::agent_type, type)
    (ast::expression, id)
    (ast::statement_list, body)
);

#endif