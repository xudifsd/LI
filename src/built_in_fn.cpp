#include "built_in_fn.h"

using namespace LI;

Add::Add()
    : BuiltInFn()
{
}

RtnValue
Add::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 1, -1);
    if (rtn.IsError())
    {
        return rtn;
    }
    float fv = 0;
    bool has_float = false;

    for (auto arg : args)
    {
        if (arg->m_type == ExpType::Integer)
        {
            const Integer& i = static_cast<const Integer&>(*arg);
            fv += i.m_value;
        }
        else if (arg->m_type == ExpType::Float)
        {
            const Float& f = static_cast<const Float&>(*arg);
            fv += f.m_value;
            has_float = true;
        }
        else
        {
            return RtnValue
            {
                RtnType::ERR_TYPE,
                "expecting int or float but got " + LI::to_string(*arg),
            };
        }
    }
    if (has_float)
    {
        result = std::make_shared<Float>(fv);
    }
    else
    {
        result = std::make_shared<Integer>(static_cast<int>(fv));
    }

    return RtnValue { RtnType::SUCC, "" };
}
