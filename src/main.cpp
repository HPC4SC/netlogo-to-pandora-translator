#include "./Expression.cpp"
#include "./Statement.cpp"
#include "./Function.cpp"
#include "./Printer.cpp"

#include <fstream>

namespace qi = boost::spirit::qi;

int main (int argc, char **argv)
{
    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
        return 1;
    }

    std::string source_code; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(source_code)
    );

    typedef std::string::const_iterator iterator_type;
    iterator_type iter = source_code.begin();
    iterator_type end = source_code.end();
    
    ast::function_list ast;
    parser::function<iterator_type> expr;

    try
    {
        bool ok = qi::phrase_parse(iter, end, expr, qi::space, ast);

        /*std::string s = testing::getString(ast);

        if (!ok)
            std::cerr << "invalid input\n";
        else
            std::cout << s << "\n";*/

    } catch (const qi::expectation_failure<iterator_type>& e)
    {
        std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
    }

    if (iter!=end) std::cerr << "unparsed: '" << std::string(iter,end) << "'\n";

    return 0;
}