#include <sstream>
#include <iostream>
#include <numeric>

#include "built_in_fn.h"

using namespace LI;

Add::Add()
    : BuiltInFn()
{
}

RtnValue
Add::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 1);
    if (rtn.IsError())
    {
        return rtn;
    }
    int iv = 0;
    float fv = 0;
    bool has_float = false;

    for (auto arg : args)
    {
        if (arg->m_type == ExpType::Integer)
        {
            const Integer& i = static_cast<const Integer&>(*arg);
            if (has_float)
            {
                fv += i.m_value;
            }
            else
            {
                iv += i.m_value;
            }
        }
        else if (arg->m_type == ExpType::Float)
        {
            const Float& f = static_cast<const Float&>(*arg);
            if (has_float)
            {
                fv += f.m_value;
            }
            else
            {
                fv = iv + f.m_value;
                has_float = true;
            }
        }
        else
        {
            std::ostringstream ss;
            ss << "expecting int or float but got " << *arg;
            return RtnValue
            {
                RtnType::ERR_TYPE,
                ss.str(),
            };
        }
    }
    if (has_float)
    {
        result = std::make_shared<Float>(fv);
    }
    else
    {
        result = std::make_shared<Integer>(iv);
    }

    return RtnValue { RtnType::SUCC, "" };
}
