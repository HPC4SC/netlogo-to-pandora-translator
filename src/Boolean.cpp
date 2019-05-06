#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;

#ifndef BOOLEAN_H
#define BOOLEAN_H

namespace Boolean {

    /********* AST *********/

    struct op_or  {};
    struct op_and {};
    struct op_xor {};
    struct op_not {};
    struct op_g   {};
    struct op_ge  {};
    struct op_l   {};
    struct op_le  {};
    struct op_e   {};
    struct op_ne  {};

    typedef std::string var;
    template <typename tag> struct binop;
    template <typename tag> struct unop;

    typedef boost::variant<var, 
            boost::recursive_wrapper<unop <op_not> >, 
            boost::recursive_wrapper<binop<op_and> >,
            boost::recursive_wrapper<binop<op_xor> >,
            boost::recursive_wrapper<binop<op_or> >,
            boost::recursive_wrapper<binop<op_g> >,
            boost::recursive_wrapper<binop<op_ge> >,
            boost::recursive_wrapper<binop<op_l> >,
            boost::recursive_wrapper<binop<op_le> >,
            boost::recursive_wrapper<binop<op_e> >,
            boost::recursive_wrapper<binop<op_ne> >
            > expr;

    template <typename tag> struct binop 
    { 
        explicit binop(const expr& l, const expr& r) : oper1(l), oper2(r) { }
        expr oper1, oper2; 
    };

    template <typename tag> struct unop  
    { 
        explicit unop(const expr& o) : oper1(o) { }
        expr oper1; 
    };

    /***********************/

    struct printer : boost::static_visitor<std::string>
    {
        std::string operator()(const var& v) const { return v; }

        std::string operator()(const binop<op_and>& b) const { return print(" & ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_or >& b) const { return print(" | ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_xor>& b) const { return print(" ^ ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_g>& b)   const { return print(" > ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_ge>& b)  const { return print(" >= ", b.oper1, b.oper2); }
        std::string operator()(const binop<op_l>& b)   const { return print(" < ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_le>& b)  const { return print(" <= ", b.oper1, b.oper2); }
        std::string operator()(const binop<op_e>& b)   const { return print(" == ", b.oper1, b.oper2); }
        std::string operator()(const binop<op_ne>& b)  const { return print(" != ", b.oper1, b.oper2); }

        std::string operator()(const unop<op_not>& u) const
        {
            return "(!" + boost::apply_visitor(*this, u.oper1) + ")";
        }

        std::string print(const std::string& op, const expr& l, const expr& r) const
        {
            return "(" + boost::apply_visitor(*this, l) + op + boost::apply_visitor(*this, r) + ")";
        }
    };

    std::string getString(const expr& e) {
        std::string s = boost::apply_visitor(printer(), e);
        return s;
    }

    /********* GRAMMAR *********/

    template <typename It, typename Skipper = qi::space_type>
        struct parser : qi::grammar<It, expr(), Skipper>
    {
        parser() : parser::base_type(expr_)
        {
            using namespace qi;

            expr_  = or_.alias();

            or_  = (xor_ >> "or"  >> or_ ) [ _val = phx::construct<binop<op_or >>(_1, _2) ] | xor_   [ _val = _1 ];
            xor_ = (and_ >> "xor" >> xor_) [ _val = phx::construct<binop<op_xor>>(_1, _2) ] | and_   [ _val = _1 ];
            and_ = (not_ >> "and" >> and_) [ _val = phx::construct<binop<op_and>>(_1, _2) ] | not_   [ _val = _1 ];
            not_ = ("not" > simple)        [ _val = phx::construct<unop <op_not>>(_1)     ] | op_     [ _val = _1 ];

            op_ =   (simple >> ">" >> op_)   [ _val = phx::construct<binop<op_g >>(_1, _2) ] |
                    (simple >> ">=" >> op_)  [ _val = phx::construct<binop<op_ge>>(_1, _2) ] |
                    (simple >> "<" >> op_)   [ _val = phx::construct<binop<op_l >>(_1, _2) ] |
                    (simple >> "<=" >> op_)  [ _val = phx::construct<binop<op_le>>(_1, _2) ] |
                    (simple >> "==" >> op_)  [ _val = phx::construct<binop<op_e >>(_1, _2) ] |
                    (simple >> "!=" >> op_)  [ _val = phx::construct<binop<op_ne>>(_1, _2) ] |
                    simple                   [qi::_val = qi::_1];
        
            simple = (('(' > expr_ > ')') | var_);
            var_ = qi::lexeme[ +alpha ];
        }

    private:
        qi::rule<It, var() , Skipper> var_;
        qi::rule<It, expr(), Skipper> op_, not_, and_, xor_, or_, simple, expr_;
    };

    /***************************/

}
/*
int main ()
{
    for (auto& input : std::list<std::string> {
            // From the OP:
            "(a and b) xor ((c and d) or (a and b));",
            "a and b xor (c and d or a and b);",
            "a and b < c xor (c > a and d < b or a and b);",

            /// Simpler tests:
            "a and b;",
            "a or b;",
            "a xor b;",
            "not a;",
            "not a and b;",
            "not (a and b);",
            "a or b or c;",
            })
    {
        auto f(std::begin(input)),   l(std::end(input));
        Boolean::parser<decltype(f)> p;

        try
        {
            Boolean::expr result;
            bool ok = qi::phrase_parse(f,l,p > ';',qi::space,result);

            if (!ok)
                std::cerr << "invalid input\n";
            else
                std::cout << "result: " << result << "\n";

        } catch (const qi::expectation_failure<decltype(f)>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (f!=l) std::cerr << "unparsed: '" << std::string(f,l) << "'\n";
    }

    return 0;
}*/

#endif