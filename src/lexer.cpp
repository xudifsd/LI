#include <iostream>

#include "lexer.h"

using namespace LI;

Lexer::Lexer(const std::string& p_fileName, std::unique_ptr<std::istream> p_stream)
    : m_fileName(p_fileName),
      m_stream(std::move(p_stream)),
      m_lineNum(1)
{
      m_colNum.emplace(1);
}

int
Lexer::NextChar()
{
    int r = m_stream->get();
    if (r == '\n')
    {
        ++m_lineNum;
        m_colNum.emplace(0);
    }
    if (r != EOF)
    {
        ++m_colNum.top();
    }
    return r;
}

void
Lexer::UngetChar(int c)
{
    m_stream->unget();
    if (c == '\n')
    {
        --m_lineNum;
        m_colNum.pop();
    }
    if (c != EOF)
    {
        --m_colNum.top();
    }
}

bool
Lexer::IsSymbol(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '<' || c == '>' || c == '=');
}

Token
Lexer::NextToken()
{
    size_t line = m_lineNum;
    size_t col = m_colNum.top();

    int c = NextChar();
    if (c == EOF)
    {
        return Token(TokenType::TEOF, line, col);
    }

    while (std::isspace(c))
    {
        line = m_lineNum;
        col = m_colNum.top();
        c = NextChar();
    }

    switch (c)
    {
        case '(':
            return Token(TokenType::LPAREN, line, col);
        case ')':
            return Token(TokenType::RPAREN, line, col);
    }

    if ((c >= '0' && c <= '9') || c == '.')
    {
        UngetChar(c);
        return BuildNumber(line, col);
    }
    else if (IsSymbol(c))
    {
        UngetChar(c);
        return BuildSymbol(line, col);
    }
    else if (c == '"')
    {
        UngetChar(c);
        return BuildString(line, col);
    }

    std::string lexeme;
    lexeme += c;
    return Token(TokenType::ERROR, line, col, lexeme);
}

Token
Lexer::BuildNumber(size_t line, size_t col)
{
    bool gotDot = false;
    std::string result;
    int c = NextChar();

    while ((c >= '0' && c <= '9') || c == '.')
    {
        result += c;
        if (c == '.')
        {
            if (gotDot)
            {
                return Token(TokenType::ERROR, line, col, result);
            }
            gotDot = true;
        }
        c = NextChar();
    }
    UngetChar(c);
    return Token(TokenType::NUMBER, line, col, result);
}

Token
Lexer::BuildSymbol(size_t line, size_t col)
{
    std::string result;
    int c = NextChar();

    while (IsSymbol(c))
    {
        result += c;
        c = NextChar();
    }
    UngetChar(c);
    return Token(TokenType::SYMBOL, line, col, result);
}

Token
Lexer::BuildString(size_t line, size_t col)
{
    std::string result;
    int c = NextChar(); // the first char is always "
    c = NextChar();

    bool hasEsc = false;

    while (c != '"' || hasEsc)
    {
        if (hasEsc)
        {
            switch (c)
            {
                case 't': c = '\t'; break;
                case 'n': c = '\n'; break;
                case '"': c = '"'; break;
                case '\\': c = '\\'; break;
                default:
                    return Token(TokenType::ERROR, line, col, result);
            }
            hasEsc = false;
            result += c;
            c = NextChar();
        }
        else
        {
            if (c == '\\')
            {
                hasEsc = true;
            }
            else if (c == '\n' || c == EOF)
            {
                return Token(TokenType::ERROR, line, col, result);
            }
            else
            {
                hasEsc = false;
                result += c;
            }
            c = NextChar();
        }
    }
    return Token(TokenType::STRING, line, col, result);
}
