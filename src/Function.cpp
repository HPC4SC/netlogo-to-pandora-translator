#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Statement.cpp"
#include "Globals.cpp"

#include <boost/spirit/include/phoenix.hpp>

namespace parser {

    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    void store_function (std::string name, std::list<std::string> args) {
        f_args.add(name, args.size());
        std::cout << name << " " <<  args.size() << std::endl;
    }

    template <typename It, typename Skipper = qi::space_type>
    struct function : qi::grammar<It, ast::function(), Skipper>
    {
        function()  : function::base_type(function_)
        {
            using namespace qi;

            name = !keywords >> !lexeme[f_args >> !(alnum | '_')] >> raw[lexeme[(alpha | '_') >> *(alnum | '_' | '-')]];

            identifier = name;
            argument_list = *identifier;

            function_ = (
                        lexeme[(string("to-report") | string("to")) >> !(alnum | '_')]  // make sure we have whole words
                    >   identifier 
                    >   ('[' > argument_list > ']')
                    >   body
                    >   lexeme[string("end") >> !(alnum | '_')]
                ) [ phx::bind(&store_function, _2, _3) ];
        }

        statement<It> body;
        qi::rule<It, std::string(), Skipper > name;
        qi::rule<It, std::string(), Skipper > identifier;
        qi::rule<It, std::list<std::string>(), Skipper > argument_list;
        qi::rule<It, ast::function(), Skipper > function_;
    };
}

#endif