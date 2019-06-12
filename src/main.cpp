#include "parser/Parser.cpp"
#include "parser/Skipper.cpp"
#include "parser/AST.hpp"
#include "processor/TypeInference.cpp"
#include "generator/FunctionGenerator.cpp"
#include "generator/MainGenerator.cpp"

#include <boost/spirit/include/qi.hpp>
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
    
    ast::main ast;
    parser::parser<iterator_type> parser;
    parser::skipper<iterator_type> skipper;

    inference::Inferer inferer;

    parser::f_args.add("random-xcor", 0);
    parser::f_args.add("random-ycor", 0);

    inference::function_types["random-xcor"] = inference::double_type;
    inference::function_types["random-ycor"] = inference::double_type;

    try
    {
        bool ok = qi::phrase_parse(iter, end, parser, skipper, ast);
        inferer(ast);
        generator::generate(ast);
 /*
        std::cout << type << std::endl;
        std::cout << "Variables:" << std::endl;
        for (auto it = inference::variable_types.begin(); it != inference::variable_types.end(); ++it) {
            std::cout << it->first << " -> " << it->second << std::endl;
        }
        std::cout << "Functions:" << std::endl;
        for (auto it = inference::function_types.begin(); it != inference::function_types.end(); ++it) {
            std::cout << it->first << " -> " << it->second << std::endl;
        }
        std::cout << "Function args:" << std::endl;
        for (auto it = inference::function_args_types.begin(); it != inference::function_args_types.end(); ++it) {
            std::cout << it->first << " -> ";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                std::cout << *it2 << " ";
            }
            std::cout << std::endl;
        }
 */
    } catch (const qi::expectation_failure<iterator_type>& e)
    {
        std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
    }

    if (iter!=end) std::cerr << "unparsed: '" << std::string(iter,end) << "'\n";

    return 0;
}