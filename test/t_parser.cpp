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
    Parser parser("<input>", string_input("Hello (+ (/ 2.123 .3 ) 122)\n abd + .2"));

    std::vector<std::shared_ptr<Expression>> exps;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        exps.push_back(e.m_exp);
        e = parser.NextExp();
    }
    exps.push_back(e.m_exp);

    ASSERT_EQ(exps.size(), 6);
    assert_sym_exp(*exps[0], "Hello");

    ASSERT_EQ(exps[1]->m_type, ExpType::List);
    const List& list = static_cast<const List&>(*exps[1]);
    ASSERT_EQ(list.m_value.size(), 3);

    assert_sym_exp(*list.m_value[0], "+");

    ASSERT_EQ(list.m_value[1]->m_type, ExpType::List);
    const List& sublist = static_cast<const List&>(*list.m_value[1]);
    ASSERT_EQ(sublist.m_value.size(), 3);
    assert_sym_exp(*sublist.m_value[0], "/");
    assert_float_exp(*sublist.m_value[1], 2.123, 0.00001);
    assert_float_exp(*sublist.m_value[2], 0.3, 0.00001);

    assert_int_exp(*list.m_value[2], 122);

    assert_sym_exp(*exps[2], "abd");
    assert_sym_exp(*exps[3], "+");
    assert_float_exp(*exps[4], 0.2, 0.00001);

    ASSERT_EQ(exps[5], nullptr);
}

TEST(testParser, failOnIncorrectFloat)
{
    Parser parser("<input>", string_input(".1.21"));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::ERROR);
    ASSERT_EQ(results[0].m_token.m_lexeme, ".1.");
}

TEST(testParser, failOnUnmatchedParentheses)
{
    Parser parser("<input>", string_input("(abd"));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::TEOF);
}

TEST(testParser, failOnUnexpectedToken)
{
    Parser parser("<input>", string_input("(.1."));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    ASSERT_EQ(results.size(), 1);
    ASSERT_TRUE(results[0].m_isError);
    ASSERT_EQ(results[0].m_token.m_type, TokenType::ERROR);
    ASSERT_EQ(results[0].m_token.m_lexeme, ".1.");
}

TEST(testParser, parseStr)
{
    Parser parser("<input>", string_input("\"Hello string\""));

    std::vector<std::shared_ptr<Expression>> exps;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        exps.push_back(e.m_exp);
        e = parser.NextExp();
    }

    ASSERT_EQ(exps.size(), 1);
    assert_str_exp(*exps[0], "Hello string");
}
