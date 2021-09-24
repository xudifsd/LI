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

bool
Token::IsError()
{
    return m_type == TokenType::ERROR;
}

std::string
LI::to_string(const TokenType& p_type)
{
    switch (p_type)
    {
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::TEOF: return "EOF";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::SYMBOL: return "SYMBOL";
        case TokenType::STRING: return "STRING";
        case TokenType::ERROR: return "ERROR";
    }
}

std::string
LI::to_string(const Token& token)
{
    return LI::to_string(token.m_type) + "{" + \
        std::to_string(token.m_lineNum) + "," + std::to_string(token.m_colNum) + "," + token.m_lexeme + "}";
}

std::ostream& LI::operator<<(std::ostream& p_stream, const Token& token)
{
    return p_stream << LI::to_string(token);
}
