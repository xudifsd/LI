#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <math.h>

#include "parser.h"
#include "setup.h"
#include "built_in_fn.h"
#include "exps.h"

using namespace LI;


std::unique_ptr<std::istream> string_input(std::string s)
{
    return std::make_unique<std::istringstream>(std::istringstream(s));
}

void assert_int_exp(const Expression& p_exp, int p_val)
{
    assert(p_exp.m_type == ExpType::Integer);
    const Integer& i = static_cast<const Integer&>(p_exp);
    assert(i.m_value == p_val);
}

void assert_float_exp(const Expression& p_exp, float b, float epsilon)
{
    assert(p_exp.m_type == ExpType::Float);
    const Float& i = static_cast<const Float&>(p_exp);
    float a = i.m_value;
    assert(fabs(a - b) <= epsilon * std::max(fabs(a), fabs(b)));
}

std::vector<std::shared_ptr<Expression>> eval(const std::string& input)
{
    std::shared_ptr<Environ> base = setup_base();
    Parser parser("<input>", string_input(input));

    std::vector<std::shared_ptr<Expression>> result;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        std::shared_ptr<Expression> r;
        RtnValue v = Eval(*e.m_exp, r, *base);
        assert(v.m_type == RtnType::SUCC);
        result.push_back(r);
        e = parser.NextExp();
    }
    return result;
}

void correct()
{
    std::vector<std::shared_ptr<Expression>> result = eval("(+ 1 2 3)");
    assert(result.size() == 1);
    assert_int_exp(*result[0], 6);

    result = eval("(+ 1 2) (+ 1) (+ 1 2 3 4 5)");
    assert(result.size() == 3);
    assert_int_exp(*result[0], 3);
    assert_int_exp(*result[1], 1);
    assert_int_exp(*result[2], 15);

    result = eval("(+ (+ 1 2) (+ (+ 1 2) 4))");
    assert(result.size() == 1);
    assert_int_exp(*result[0], 10);

    result = eval("(+ (+ 1.1 2.2))");
    assert(result.size() == 1);
    assert_float_exp(*result[0], 3.3, 0.00001);
}

int main()
{
    correct();
    return 0;
}
