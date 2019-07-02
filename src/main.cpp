#define _GLIBCXX_USE_CXX11_ABI 0

#include "parser/Parser.cpp"
#include "parser/Skipper.cpp"
#include "parser/AST.hpp"

#include "generator/FunctionGenerator.cpp"
#include "generator/MainGenerator.cpp"

#include "processor/TypeInference.cpp"
#include "processor/AgentActions.cpp"
#include "processor/ScanSetupFunction.cpp"

#include <boost/spirit/include/qi.hpp>
#include <fstream>

namespace qi = boost::spirit::qi;

int main (int argc, char **argv)
{
    char const* filename;
    std::string outputDir;
    if (argc > 2)
    {
        filename = argv[1];
        outputDir = argv[2];
    }
    else
    {
        std::cerr << "Error: No input file or output directory provided." << std::endl;
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

    processor::Inferer inferer;

    parser::f_args.add("random-xcor", 0);
    parser::f_args.add("random-ycor", 0);
    parser::f_args.add("reproduce", 0);

    processor::function_types["random-xcor"] = processor::double_type;
    processor::function_types["random-ycor"] = processor::double_type;
    processor::function_types["reproduce"] = processor::void_type;

    try
    {
        bool ok = qi::phrase_parse(iter, end, parser, skipper, ast);
        f_list = ast.functions;
        inferer(ast);
        processor::scanSetupFunction(ast);
        processor::scanAgentActions(ast);
        generator::generate(ast, outputDir);

    } catch (const qi::expectation_failure<iterator_type>& e)
    {
        std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
    }

    if (iter!=end) std::cerr << "unparsed: '" << std::string(iter,end) << "'\n";

    return 0;
}