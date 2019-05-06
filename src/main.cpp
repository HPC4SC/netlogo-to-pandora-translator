#include "./Arithmetic.cpp"
 
int main ()
{
    for (auto& input : std::list<std::string> {
            // From the OP:
            "(a + b) - ((c + d) / (a - b))",
            "a + b * (c - d / a * b)",
            "5 + 3.5 + c + (c * a * d * b + a * b)",
            })
    {
        auto f(std::begin(input)), l(std::end(input));
        Arithmetic::parser<decltype(f)> p;

        try
        {
            Arithmetic::expr result;
            bool ok = qi::phrase_parse(f, l, p, qi::space, result);
            std::string s = Arithmetic::getString(result);

            if (!ok)
                std::cerr << "invalid input\n";
            else
                std::cout << s << "\n";

        } catch (const qi::expectation_failure<decltype(f)>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (f!=l) std::cerr << "unparsed: '" << std::string(f,l) << "'\n";
    }

    return 0;
}