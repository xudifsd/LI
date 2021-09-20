#include "env.h"

using namespace LI;

Environ::Environ(std::shared_ptr<Environ> p_parent)
    : m_parent(p_parent)
{
}

std::shared_ptr<Expression>
Environ::Lookup(const std::string& p_symbol)
{
    Environ* p = this;
    while (p != nullptr)
    {
        auto result = p->m_current.find(p_symbol);
        if (result != p->m_current.end())
        {
            return result->second;
        }
        p = p->m_parent.get();
    }
    return nullptr;
}

bool
Environ::Set(const std::string& p_symbol, std::shared_ptr<Expression> value)
{
    auto previous = m_current.find(p_symbol);
    m_current[p_symbol] = value;
    return previous == m_current.end();
}
