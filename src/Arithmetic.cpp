#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

namespace Arithmetic {

    /********* AST *********/

    struct op_sum {};
    struct op_min {};
    struct op_div {};
    struct op_mul {};

    typedef std::string var;
    template <typename tag> struct binop;

    typedef boost::variant<
                var,
                double,
                boost::recursive_wrapper<binop<op_sum> >, 
                boost::recursive_wrapper<binop<op_min> >,
                boost::recursive_wrapper<binop<op_div> >,
                boost::recursive_wrapper<binop<op_mul> >
            > expr;

    template <typename tag> struct binop 
    { 
        explicit binop(const expr& l, const expr& r) : oper1(l), oper2(r) { }
        expr oper1, oper2; 
    };

    /***********************/

    struct printer : boost::static_visitor<std::string>
    {
        printer(std::string& s) : _s(s) {}
        std::string& _s;

        //
        std::string operator()(const var& v) const { return v; }
        std::string operator()(const double& v) const { return boost::lexical_cast<std::string>(v);; }

        std::string operator()(const binop<op_sum>& b) const { return print(" + ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_min>& b) const { return print(" - ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_mul>& b) const { return print(" * ",  b.oper1, b.oper2); }
        std::string operator()(const binop<op_div>& b) const { return print(" / ",  b.oper1, b.oper2); }

        std::string print(const std::string& op, const expr& l, const expr& r) const
        {
            return "(" + boost::apply_visitor(*this, l) + op + boost::apply_visitor(*this, r) + ")";
        }
    };

    std::string getString(const expr& e) {
        std::string s;
        s = boost::apply_visitor(printer(s), e);
        return s;
    }

    /********* GRAMMAR *********/

    template <typename It, typename Skipper = qi::space_type>
        struct parser : qi::grammar<It, expr(), Skipper>
    {
        parser() : parser::base_type(start)
        {
            using namespace qi;

            start = (product >> '+' >> start)       [ _val = phx::construct<binop<op_sum>>(qi::_1, qi::_2) ] |
                    (product >> '-' >> start)       [ _val = phx::construct<binop<op_min>>(qi::_1, qi::_2) ] |
                    product                         [ _val = qi::_1 ];
            product = (factor >> '*' >> product)    [ _val = phx::construct<binop<op_mul>>(qi::_1, qi::_2) ] |
                    (factor >> '/' >> product)    [ _val = phx::construct<binop<op_div>>(qi::_1, qi::_2) ] |
                    factor                        [ _val = qi::_1 ];
            factor = group                          [ _val = qi::_1 ] |
                    qi::double_                    [ _val = qi::_1 ] |
                    var_                           [ _val = qi::_1 ];
            group %= '(' >> start >> ')';
            var_ = qi::lexeme[ +alpha ];
        }

    private:
        qi::rule<It, var() , Skipper> var_;
        qi::rule<It, expr(), Skipper> start, group, product, factor;
    };

    /***************************/

}
/*
int main ()
{
    for (auto& input : std::list<std::string> {
            // From the OP:
            "(a + b) - ((c + d) / (a - b));",
            "a + b * (c - d / a * b);",
            "5 + 3.5 + c + (c * a * d * b + a * b);",
            })
    {
        auto f(std::begin(input)), l(std::end(input));
        Arithmetic::parser<decltype(f)> p;

        try
        {
            Arithmetic::expr result;
            bool ok = qi::phrase_parse(f, l, p > ';', qi::space, result);

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
}
*/
#endif