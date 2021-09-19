#include "types.h"

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

List::List(const std::vector<Expression> p_val)
    : Expression(ExpType::List),
      m_value(p_val)
{
}
