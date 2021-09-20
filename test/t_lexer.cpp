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
    std::string input = "Hello (+ (/ 2.123 .3 ) 122)\n abd + .2";
    std::unique_ptr<std::istream> p = std::make_unique<std::istringstream>(std::istringstream(input));
    Lexer lexer("<input>", std::move(p));

    std::vector<Token> tokens;
    Token t = lexer.NextToken();
    while (t.m_type != TokenType::ERROR && t.m_type != TokenType::TEOF)
    {
        tokens.push_back(t);
        t = lexer.NextToken();
    }
    tokens.push_back(t);

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
    std::string input = "Hello .1.21";
    std::unique_ptr<std::istream> p = std::make_unique<std::istringstream>(std::istringstream(input));
    Lexer lexer("<input>", std::move(p));

    std::vector<Token> tokens;
    Token t = lexer.NextToken();
    while (t.m_type != TokenType::ERROR && t.m_type != TokenType::TEOF)
    {
        tokens.push_back(t);
        t = lexer.NextToken();
    }
    tokens.push_back(t);

    ASSERT_EQ(tokens.size(), 2);
    assert_token(tokens[0], TokenType::SYMBOL, 1, 1, "Hello");
    assert_token(tokens[1], TokenType::ERROR, 1, 7, ".1.");
}
