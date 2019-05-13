#include "./Expression.cpp"
//#include "./Printer.cpp"
 
#include <string>

int main ()
{
    for (auto& input : std::list<std::string> {
            // From the OP:
            "(a + b)",
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
            /*
            std::string s = printer::getString(result);

            if (!ok)
                std::cerr << "invalid input\n";
            else
                std::cout << s << "\n";
*/
        } catch (const qi::expectation_failure<iterator_type>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (iter!=end) std::cerr << "unparsed: '" << std::string(iter,end) << "'\n";
    }

    return 0;
}