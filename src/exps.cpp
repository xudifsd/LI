#include <iostream>

#include "exps.h"

using namespace LI;

Expression::Expression(ExpType p_type)
    : m_type(p_type)
{
}

Expression::~Expression()
{
}

Integer::Integer(int p_val)
    : Expression(ExpType::Integer),
      m_value(p_val)
{
}

Float::Float(float p_val)
    : Expression(ExpType::Float),
      m_value(p_val)
{
}

Symbol::Symbol(const std::string& p_val)
    : Expression(ExpType::Symbol),
      m_value(p_val)
{
}

List::List(const std::vector<std::shared_ptr<Expression>> p_val)
    : Expression(ExpType::List),
      m_value(p_val)
{
}

std::ostream& operator<<(std::ostream& p_stream, const Expression& p_exp)
{
    switch (p_exp.m_type)
    {
        case ExpType::Integer:
        {
            p_stream << "Integer:";
            const Integer& i = static_cast<const Integer&>(p_exp);
            p_stream << std::to_string(i.m_value);
            break;
        }
        case ExpType::Float:
        {
            p_stream << "Float:";
            const Float& f = static_cast<const Float&>(p_exp);
            p_stream << std::to_string(f.m_value);
            break;
        }
        case ExpType::Symbol:
        {
            p_stream << "Symbol:";
            const Symbol& s = static_cast<const Symbol&>(p_exp);
            p_stream << s.m_value;
            break;
        }
        case ExpType::List:
        {
            p_stream << "List:";
            const List& l = static_cast<const List&>(p_exp);

            p_stream << "(";
            for (int i = 0; i < l.m_value.size(); ++i)
            {
                p_stream << *l.m_value[i];
                if (i != l.m_value.size() - 1)
                {
                    p_stream << ",";
                }
            }
            p_stream << ")";
            break;
        }
        case ExpType::Callable:
        {
            p_stream << "Callable";
            break;
        }
    }
    return p_stream;
}
