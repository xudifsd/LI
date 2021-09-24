#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <gtest/gtest.h>

#include "parser.h"
#include "token.h"
#include "exps.h"
#include "test_utils.h"

using namespace LI;
using namespace LI_test;

TEST(testParser, correct)
{
    std::vector<ParseResult> results = parse("Hello (+ (/ 2.123 .3 ) 122)\n abd + .2");

    ASSERT_EQ(results.size(), 6);
    assert_sym_exp(results[0], "Hello");

    ASSERT_FALSE(results[1].m_isError);
    ASSERT_EQ(results[1].m_exp->m_type, ExpType::List);
    const List& list = static_cast<const List&>(*results[1].m_exp);
    ASSERT_EQ(list.m_value.size(), 3);

    assert_sym_exp(*list.m_value[0], "+");

    ASSERT_EQ(list.m_value[1]->m_type, ExpType::List);
    const List& sublist = static_cast<const List&>(*list.m_value[1]);
    ASSERT_EQ(sublist.m_value.size(), 3);
    assert_sym_exp(*sublist.m_value[0], "/");
    assert_float_exp(*sublist.m_value[1], 2.123, 0.00001);
    assert_float_exp(*sublist.m_value[2], 0.3, 0.00001);

    assert_int_exp(*list.m_value[2], 122);

    assert_sym_exp(results[2], "abd");
    assert_sym_exp(results[3], "+");
    assert_float_exp(results[4], 0.2, 0.00001);

    assert_eof_exp(results[5]);
}

TEST(testParser, failOnIncorrectFloat)
{
    std::vector<ParseResult> results = parse(".1.21");

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::ERROR);
    ASSERT_EQ(results[0].m_token.m_lexeme, ".1.");
}

TEST(testParser, failOnUnmatchedParentheses)
{
    std::vector<ParseResult> results = parse("(abd");

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::TEOF);
}

TEST(testParser, failOnUnexpectedToken)
{
    std::vector<ParseResult> results = parse("(.1.");

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::ERROR);
    ASSERT_EQ(results[0].m_token.m_lexeme, ".1.");
}

TEST(testParser, parseStr)
{
    std::vector<ParseResult> results = parse("\"Hello string\"");

    ASSERT_EQ(results.size(), 2);
    assert_str_exp(results[0], "Hello string");
    assert_eof_exp(results[1]);
}
