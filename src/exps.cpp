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

std::string
LI::to_string(const LI::ExpType& expType)
{
    switch (expType)
    {
        case ExpType::Integer: return "Integer";
        case ExpType::Float: return "Float";
        case ExpType::Symbol: return "Symbol";
        case ExpType::List: return "List";
        case ExpType::Callable: return "Callable";
    }
}

std::string
LI::to_string(const LI::Expression& exp)
{
    std::string result = LI::to_string(exp.m_type);
    switch (exp.m_type)
    {
        case ExpType::Integer:
        {
            const Integer& i = static_cast<const Integer&>(exp);
            return result + ":" + std::to_string(i.m_value);
        }
        case ExpType::Float:
        {
            const Float& f = static_cast<const Float&>(exp);
            return result + ":" + std::to_string(f.m_value);
        }
        case ExpType::Symbol:
        {
            const Symbol& s = static_cast<const Symbol&>(exp);
            return result + ":" + s.m_value;
        }
        case ExpType::List:
        {
            const List& l = static_cast<const List&>(exp);

            result += ":(";
            for (int i = 0; i < l.m_value.size(); ++i)
            {
                result += LI::to_string(*l.m_value[i]);
                if (i != l.m_value.size() - 1)
                {
                    result += ",";
                }
            }
            return result + ")";
        }
        case ExpType::Callable:
        {
            return result;
        }
    }
}

std::ostream& LI::operator<<(std::ostream& p_stream, const Expression& p_exp)
{
    return p_stream << LI::to_string(p_exp);
}
