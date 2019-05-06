#define BOOST_TEST_MODULE dataset_example68
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <sstream>
#include <iostream>

#include "./../src/Arithmetic.cpp"

namespace bdata = boost::unit_test::data;

const std::list<std::string> test_input = {
    "(a + b) - (c + d) / (a - b)",
    "a + b * (c - d / a * b)",
    "5 + 3.5 + c + (c * a * d * b + a * b)",
};

const std::list<std::string> test_output = {
    "((a + b) - ((c + d) / (a - b)))",
    "(a + (b * (c - (d / (a * b)))))",
    "(5 + (3.5 + (c + ((c * (a * (d * b))) + (a * b)))))",
};

BOOST_DATA_TEST_CASE( arithmetic_case_1, bdata::make(test_input) ^ bdata::make(test_output), input, output ) {
    auto f(std::begin(input)), l(std::end(input));

    Arithmetic::parser<decltype(f)> p;
    Arithmetic::expr result;

    bool ok = qi::phrase_parse(f, l, p, qi::space, result);
    std::string str = Arithmetic::getString(result);
    BOOST_TEST_MESSAGE( str );

    BOOST_TEST((str == output));
}