#pragma once

#include <iostream>
#include <memory>

#include "lexer.h"
#include "exps.h"

namespace LI
{
    struct ParseResult
    {
        bool m_isError;
        Token m_token;
        std::shared_ptr<Expression> m_exp;
    };

    class Parser
    {
        public:
            Parser(const std::string& p_fileName, std::unique_ptr<std::istream> p_stream);

            ParseResult NextExp();

        private:
            static std::shared_ptr<Expression> ToExp(const Token& t);
            Lexer m_lexer;
    };
}
