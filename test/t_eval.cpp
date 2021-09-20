#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <gtest/gtest.h>

#include "parser.h"
#include "setup.h"
#include "built_in_fn.h"
#include "exps.h"
#include "test_utils.h"

using namespace LI;
using namespace LI_test;

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
        //ASSERT_EQ(v.m_type, RtnType::SUCC);
        result.push_back(r);
        e = parser.NextExp();
    }
    return result;
}

TEST(testEval, correct)
{
    std::vector<std::shared_ptr<Expression>> result = eval("(+ 1 2 3)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 6);

    result = eval("(+ 1 2) (+ 1) (+ 1 2 3 4 5)");
    ASSERT_EQ(result.size(), 3);
    assert_int_exp(*result[0], 3);
    assert_int_exp(*result[1], 1);
    assert_int_exp(*result[2], 15);

    result = eval("(+ (+ 1 2) (+ (+ 1 2) 4))");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 10);

    result = eval("(+ (+ 1.1 2.2))");
    ASSERT_EQ(result.size(), 1);
    assert_float_exp(*result[0], 3.3, 0.00001);
}
