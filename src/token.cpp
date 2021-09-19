#include "token.h"

using namespace LI;

Token::Token(TokenType p_type, size_t p_lineNum, size_t p_colNum)
    : m_type(p_type),
      m_lineNum(p_lineNum),
      m_colNum(p_colNum)
{
}

Token::Token(TokenType p_type, size_t p_lineNum, size_t p_colNum, const std::string& p_lexeme)
    : m_type(p_type),
      m_lineNum(p_lineNum),
      m_colNum(p_colNum),
      m_lexeme(p_lexeme)
{
}

std::ostream& operator<<(std::ostream& p_stream, const Token& p_token)
{
    std::string t;
    switch (p_token.m_type)
    {
        case TokenType::LPAREN:
            t = "LPAREN";
            break;
        case TokenType::RPAREN:
            t = "RPAREN";
            break;
        case TokenType::TEOF:
            t = "EOF";
            break;
        case TokenType::NUMBER:
            t = "NUMBER";
            break;
        case TokenType::SYMBOL:
            t = "SYMBOL";
            break;
        case TokenType::ERROR:
            t = "ERROR";
            break;
            /*
        case TokenType::LBRACE:
            t = "LBRACE";
            break;
        case TokenType::RBRACE:
            t = "RBRACE";
            break;
            */
    }
    return p_stream << t << "{" << p_token.m_lineNum << "," << p_token.m_colNum << "," << p_token.m_lexeme << "}";
}
