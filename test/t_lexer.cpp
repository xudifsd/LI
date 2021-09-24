#include <iostream>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

#include "lexer.h"
#include "token.h"
#include "test_utils.h"

using namespace LI;
using namespace LI_test;

TEST(testLexer, correct)
{
    std::vector<Token> tokens = LI_test::tokenize("Hello (+ (/ 2.123 .3 ) 122)\n abd + .2");

    ASSERT_EQ(tokens.size(), 14);
    assert_token(tokens[0], TokenType::SYMBOL, 1, 1, "Hello");
    assert_token(tokens[1], TokenType::LPAREN, 1, 7, "");
    assert_token(tokens[2], TokenType::SYMBOL, 1, 8, "+");
    assert_token(tokens[3], TokenType::LPAREN, 1, 10, "");
    assert_token(tokens[4], TokenType::SYMBOL, 1, 11, "/");
    assert_token(tokens[5], TokenType::NUMBER, 1, 13, "2.123");
    assert_token(tokens[6], TokenType::NUMBER, 1, 19, ".3");
    assert_token(tokens[7], TokenType::RPAREN, 1, 22, "");
    assert_token(tokens[8], TokenType::NUMBER, 1, 24, "122");
    assert_token(tokens[9], TokenType::RPAREN, 1, 27, "");

    assert_token(tokens[10], TokenType::SYMBOL, 2, 2, "abd");
    assert_token(tokens[11], TokenType::SYMBOL, 2, 6, "+");
    assert_token(tokens[12], TokenType::NUMBER, 2, 8, ".2");
    assert_token(tokens[13], TokenType::TEOF, 2, 10, "");
}

TEST(testLexer, failOnIncorrectFloat)
{
    std::vector<Token> tokens = LI_test::tokenize("Hello .1.21");

    ASSERT_EQ(tokens.size(), 2);
    assert_token(tokens[0], TokenType::SYMBOL, 1, 1, "Hello");
    assert_token(tokens[1], TokenType::ERROR, 1, 7, ".1.");
}

TEST(testLexer, string)
{
    std::vector<Token> tokens = LI_test::tokenize("\"Hello string\"");

    ASSERT_EQ(tokens.size(), 2);
    assert_token(tokens[0], TokenType::STRING, 1, 1, "Hello string");
    assert_token(tokens[1], TokenType::TEOF, 1, 15, "");
}

TEST(testLexer, stringWithEsc)
{
    std::vector<Token> tokens = LI_test::tokenize("\"Hello \\\"esc\"");

    ASSERT_EQ(tokens.size(), 2);
    assert_token(tokens[0], TokenType::STRING, 1, 1, "Hello \"esc");
    assert_token(tokens[1], TokenType::TEOF, 1, 14, "");

    tokens = LI_test::tokenize("\"\\n\"");

    ASSERT_EQ(tokens.size(), 2);
    assert_token(tokens[0], TokenType::STRING, 1, 1, "\n");
    assert_token(tokens[1], TokenType::TEOF, 1, 5, "");
}

TEST(testLexer, newNewLineInStr)
{
    std::vector<Token> tokens = LI_test::tokenize("\"first line\nseconde line\"");
    ASSERT_EQ(tokens.size(), 1);
    assert_token(tokens[0], TokenType::ERROR, 1, 1, "first line");
}
