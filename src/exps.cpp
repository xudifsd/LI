#include <iostream>

#include "exps.h"

using namespace LI;

Expression::Expression(ExpType p_type)
    : m_type(p_type)
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

Symbol::Symbol(const std::string p_val)
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
    std::string t;
    std::string content;
    switch (p_exp.m_type)
    {
        case ExpType::Integer:
        {
            t = "Integer";
            const Integer& i = static_cast<const Integer&>(p_exp);
            content += std::to_string(i.m_value);
            break;
        }
        case ExpType::Float:
        {
            t = "Float";
            const Float& f = static_cast<const Float&>(p_exp);
            content += std::to_string(f.m_value);
            break;
        }
        case ExpType::Symbol:
        {
            t = "Symbol";
            const Symbol& s = static_cast<const Symbol&>(p_exp);
            content += s.m_value;
            break;
        }
        case ExpType::List:
        {
            t = "List";
            //const List& l = static_cast<const List&>(p_exp);
            //content += std::to_string(s.m_value);
            break;
        }
    }
    return p_stream << t << ":" << content;
}
