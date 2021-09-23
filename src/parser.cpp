#include <vector>
#include <stack>
#include <memory>

#include "parser.h"
#include "token.h"

using namespace LI;

Parser::Parser(const std::string& p_fileName, std::unique_ptr<std::istream> p_stream)
    : m_lexer(p_fileName, std::move(p_stream))
{
}


std::shared_ptr<Expression>
Parser::ToExp(const Token& t)
{
    if (t.m_type == TokenType::NUMBER)
    {
        if (t.m_lexeme.find('.') == std::string::npos)
        {
            return std::make_shared<Integer>(std::stoi(t.m_lexeme));
        }
        return std::make_shared<Float>(std::stof(t.m_lexeme));
    }
    else if (t.m_type == TokenType::SYMBOL)
    {
        return std::make_shared<Symbol>(t.m_lexeme);
    }
    return nullptr;
}


ParseResult
Parser::NextExp()
{
    Token t = m_lexer.NextToken();

    switch (t.m_type)
    {
        case TokenType::ERROR:
            return ParseResult { true, t, nullptr };
        case TokenType::TEOF:
            return ParseResult { false, t, nullptr };
        case TokenType::NUMBER:
        case TokenType::SYMBOL:
            return ParseResult { false, t, ToExp(t) };
        case TokenType::RPAREN:
            return ParseResult { true, t, nullptr };
        case TokenType::LPAREN:
        {
            std::stack<std::vector<std::shared_ptr<Expression>>> stack;
            stack.push(std::vector<std::shared_ptr<Expression>>());

            for (;;)
            {
                t = m_lexer.NextToken();
                switch (t.m_type)
                {
                    case TokenType::ERROR:
                    case TokenType::TEOF:
                        return ParseResult { true, t, nullptr };
                        return ParseResult { true, t, nullptr };
                    case TokenType::NUMBER:
                    case TokenType::SYMBOL:
                        stack.top().push_back(ToExp(t));
                        break;
                    case TokenType::RPAREN:
                        {
                            auto top = stack.top();
                            stack.pop();
                            if (stack.size() == 0)
                            {
                                return ParseResult { false, t, std::make_shared<List>(top) };
                            }
                            stack.top().push_back(std::make_shared<List>(top));
                            break;
                        }
                    case TokenType::LPAREN:
                        stack.push(std::vector<std::shared_ptr<Expression>>());
                        break;
                }
            }
        }
    }
}
