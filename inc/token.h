#pragma once

#include <string>
#include <iostream>

namespace LI
{
    enum class TokenType
    {
        LPAREN,  // "("
        RPAREN,  // ")"
        TEOF,    // EOF
        NUMBER,  // number
        SYMBOL,  // symbol
        //LBRACE,  // "{"
        //RBRACE,  // "}"
        ERROR,   // for error handling
    };

    struct Token
    {
        TokenType m_type;
        size_t m_lineNum;
        size_t m_colNum;
        std::string m_lexeme;

        Token(TokenType p_type, size_t p_lineNum, size_t p_colNum);
        Token(TokenType p_type, size_t p_lineNum, size_t p_colNum, const std::string& p_lexeme);
    };
}

std::ostream& operator<<(std::ostream& p_stream, const LI::Token& p_token);

