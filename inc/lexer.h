#pragma once

#include <string>
#include <memory>
#include <stack>

#include "token.h"

namespace LI
{
    class Lexer
    {
        public:
            Lexer(const std::string& p_fileName, std::unique_ptr<std::istream> p_stream);
            Token NextToken();

        private:
            int NextChar();
            void UngetChar(int c);
            bool IsSymbol(int c);

            Token BuildNumber(size_t line, size_t col);
            Token BuildSymbol(size_t line, size_t col);

            std::string m_fileName;
            std::unique_ptr<std::istream> m_stream;
            size_t m_lineNum;
            std::stack<size_t> m_colNum;
    };
}
