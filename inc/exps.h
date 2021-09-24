#pragma once

#include <string>
#include <vector>
#include <memory>
#include <ostream>

namespace LI
{
    enum class ExpType
    {
        Integer,
        Float,
        Symbol,
        String,
        List,
        Callable,
        Nil,
    };

    struct Expression
    {
        Expression(ExpType p_type);

        ExpType m_type;

        virtual ~Expression();
    };

    struct Integer : Expression
    {
        Integer(int p_val);

        int m_value;
    };

    struct Float : Expression
    {
        Float(float p_val);

        float m_value;
    };

    struct Symbol : Expression
    {
        Symbol(const std::string& p_val);

        std::string m_value;
    };

    struct List : Expression
    {
        List(const std::vector<std::shared_ptr<Expression>> p_val);

        std::vector<std::shared_ptr<Expression>> m_value;
    };

    struct Nil : Expression
    {
        Nil();
    };

    struct String : Expression
    {
        String(const std::string& p_val);

        std::string m_value;
    };

    std::string to_string(const LI::ExpType& expType);
    std::string to_string(const LI::Expression& exp);
    std::ostream& operator<<(std::ostream& p_stream, const LI::Expression& p_exp);
}

