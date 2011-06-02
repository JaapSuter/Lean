// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/expression_builder.hpp"
#include "lean/exception.hpp"
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/utility/chset_operators.hpp>
#include <string>

namespace spirit = boost::spirit;

namespace lean {

namespace
{
    struct accept_bool_t
    {
        expression_builder_t* _eb;
        accept_bool_t(expression_builder_t& eb) : _eb(&eb) {}
        
        void operator() (const char* begin, const char* end) const
        {
            std::string token(begin, end);
            if ("true" == token)
                _eb->add(true);
            else
            if ("false" == token)
                _eb->add(false);
            else
            {
                assert(false && "unreachable code, not a proper boolean value");
            }
            
        }
    };

    struct accept_paren_t
    {
        expression_builder_t* _eb;
        accept_paren_t(expression_builder_t& eb) : _eb(&eb) {}
        
        void operator() (const char paren) const
        {
            if (paren == '(')
                _eb->open_paren();
            else
            if (paren == ')')
                _eb->close_paren();
            else
            {
                assert(false && "unreachable code, not a well formed s-expression");
            }
            
        }
    };

    struct accept_string_t
    {
        expression_builder_t* _eb;
        accept_string_t(expression_builder_t& eb) : _eb(&eb) {}

        void operator() (const char* begin, const char* end) const
        {
            _eb->add(std::string(begin, end));
        }
    };

    struct accept_identifier_t
    {
        expression_builder_t* _eb;
        accept_identifier_t(expression_builder_t& eb) : _eb(&eb) {}

        void operator() (const char* begin, const char* end) const
        {
            _eb->add(identifier_t(std::string(begin, end)));
        }
    };

    struct accept_number_t
    {
        expression_builder_t* _eb;
        accept_number_t(expression_builder_t& eb) : _eb(&eb) {}

        void operator () (double d) const
        {
            _eb->add(d);
        }
    };

	struct accept_lambda_t
    {
        expression_builder_t* _eb;
        accept_lambda_t(expression_builder_t& eb) : _eb(&eb) {}

		template<class T>
        void operator () (const T*, const T*) const
        {
            _eb->add(lambda());
        }
    };
}

value_t parse(const char* str)
{
    expression_builder_t eb;
    
    spirit::rule<> comments = spirit::comment_p("/*", "*/") | spirit::comment_p("//");
    spirit::rule<> whitespace  =  spirit::blank_p | spirit::eol_p | comments;

	spirit::rule<> open_paren  = spirit::ch_p('(')[accept_paren_t(eb)];
    spirit::rule<> close_paren = spirit::ch_p(')')[accept_paren_t(eb)];	

    spirit::chset<char> chars_not_in_identifier = spirit::chset<char>("()'\"");
    spirit::rule<> identifier = (((spirit::graph_p - chars_not_in_identifier) - spirit::digit_p)
                            >> *(spirit::graph_p - chars_not_in_identifier))[accept_identifier_t(eb)];
    
    spirit::rule<> boolean = spirit::str_p("true")[accept_bool_t(eb)] | spirit::str_p("false")[accept_bool_t(eb)];

	spirit::rule<> lambda = spirit::str_p("lambda")[accept_lambda_t(eb)];

    spirit::rule<> number = spirit::real_p[accept_number_t(eb)];    
    spirit::rule<> string = spirit::confix_p('"', (*spirit::c_escape_ch_p)[accept_string_t(eb)], '"');

    spirit::rule<> cell = lambda
						| number // number must come first to match number signs before subtraction function
                        | boolean
                        | string
                        | identifier;

	spirit::rule<> sexp;
    spirit::rule<> list = *whitespace >> open_paren >> *whitespace 
                       >> *(sexp | cell) >> *(+whitespace >> *(sexp | cell))
                       >> *whitespace >> close_paren >> *whitespace;

	sexp = cell | list;

    spirit::parse_info<const char*> pi = spirit::parse
    (
        str,
        sexp
    );
    
    if (!pi.full)
        throw parse_error_t(pi.stop);
    else 
        return eb.get();    
}

}
