#pragma once

#include <string>
#include <vector>

namespace LI
{
    enum class ExpType
    {
        Integer,
        Float,
        Symbol,
        List,
    };

    struct Expression
    {
        Expression(ExpType p_type);

        ExpType m_type;
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
        Symbol(const std::string p_val);

        std::string m_value;
    };

    struct List : Expression
    {
        List(const std::vector<Expression> p_val);

        std::vector<Expression> m_value;
    };
}
