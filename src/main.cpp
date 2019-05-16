#include "./Expression.cpp"
#include "./Printer.cpp"

namespace qi = boost::spirit::qi;

int main ()
{
    for (auto& input : std::list<std::string> {
            // From the OP:
            
            "(a + b) - ((c + d) / (a - b))",
            "a + b * (c - d / a * b)",
            "5 + 3.5 + c + (c * a * d * b + a * b)",
            "(a and b) or ((c and d) or (a and b))",
            "a and multiply(a*b, true) or (c and d or a and b)",
            "a and b < c or (c > (a + b) - ((c + d) / (a - b)) and d < b or a and b)",
            /// Simpler tests:
            "a and b",
            "a or b",
            "a or b",
            "not a",
            "not a and b",
            "not (a and b)",
            "a or b or c"
    })
    {
        typedef std::string::const_iterator iterator_type;
        iterator_type iter = input.begin();
        iterator_type end = input.end();
        
        ast::expression ast;
        parser::expression<iterator_type> expr;

        try
        {
            bool ok = qi::phrase_parse(iter, end, expr, qi::space, ast);

            std::string s = testing::getString(ast);

            if (!ok)
                std::cerr << "invalid input\n";
            else
                std::cout << s << "\n";

        } catch (const qi::expectation_failure<iterator_type>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (iter!=end) std::cerr << "unparsed: '" << std::string(iter,end) << "'\n";
    }

    return 0;
}