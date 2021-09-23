#include <iostream>

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
        auto result = p->m_frame.find(p_symbol);
        if (result != p->m_frame.end())
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
    auto previous = m_frame.find(p_symbol);
    m_frame[p_symbol] = value;
    return previous == m_frame.end();
}

std::ostream& LI::operator<<(std::ostream& p_stream, const LI::Environ& p_env)
{
    p_stream << "Environ:{";
    for (auto pair : p_env.m_frame)
    {
        p_stream << pair.first << ":" << *pair.second << ",";
    }
    p_stream << "}";
    if (p_env.m_parent != nullptr)
    {
        p_stream << "->" << *p_env.m_parent;
    }
    return p_stream;
}
