#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;

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

    struct printer : boost::static_visitor<void>
    {
        printer(std::ostream& os) : _os(os) {}
        std::ostream& _os;

        //
        void operator()(const var& v) const { _os << v; }
        void operator()(const double& v) const { _os << v; }

        void operator()(const binop<op_sum>& b) const { print(" + ",  b.oper1, b.oper2); }
        void operator()(const binop<op_min>& b) const { print(" - ",  b.oper1, b.oper2); }
        void operator()(const binop<op_mul>& b) const { print(" * ",  b.oper1, b.oper2); }
        void operator()(const binop<op_div>& b) const { print(" / ",  b.oper1, b.oper2); }

        void print(const std::string& op, const expr& l, const expr& r) const
        {
            _os << "(";
                boost::apply_visitor(*this, l);
                _os << op;
                boost::apply_visitor(*this, r);
            _os << ")";
        }
    };

    std::ostream& operator<<(std::ostream& os, const expr& e)
    { boost::apply_visitor(printer(os), e); return os; }

    /********* GRAMMAR *********/

    template <typename It, typename Skipper = qi::space_type>
        struct parser : qi::grammar<It, expr(), Skipper>
    {
        parser() : parser::base_type(start)
        {
            using namespace qi;

            start = (product >> '+' >> start)       [ _val = phx::construct<binop<op_sum>>(_1, _2) ] |
                    (product >> '-' >> start)       [ _val = phx::construct<binop<op_min>>(_1, _2) ] |
                    product                         [ _val = _1 ];
            product = (factor >> '*' >> product)    [ _val = phx::construct<binop<op_mul>>(_1, _2) ] |
                    (factor >> '/' >> product)    [ _val = phx::construct<binop<op_div>>(_1, _2) ] |
                    factor                        [ _val = _1 ];
            factor = group                          [ _val = _1 ] |
                    qi::double_                    [ _val = _1 ] |
                    var_                           [ _val = _1 ];
            group %= '(' >> start >> ')';
            var_ = qi::lexeme[ +alpha ];
        }

    private:
        qi::rule<It, var() , Skipper> var_;
        qi::rule<It, expr(), Skipper> start, group, product, factor;
    };

    /***************************/

}

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
}