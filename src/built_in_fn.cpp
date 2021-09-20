#include <sstream>
#include <iostream>
#include <numeric>

#include "built_in_fn.h"

using namespace LI;

RtnValue
LI::Eval(const Expression& exp, std::shared_ptr<Expression>& result, Environ& env)
{
    switch (exp.m_type)
    {
        case ExpType::Integer:
        {
            const Integer& i = static_cast<const Integer&>(exp);
            result = std::make_shared<Integer>(i.m_value);
            return RtnValue { RtnType::SUCC, "" };
        }
        case ExpType::Float:
        {
            const Float& f = static_cast<const Float&>(exp);
            result = std::make_shared<Float>(f.m_value);
            return RtnValue { RtnType::SUCC, "" };
        }
        case ExpType::Symbol:
        {
            const Symbol& s = static_cast<const Symbol&>(exp);
            result = env.Lookup(s.m_value);
            if (result == nullptr)
            {
                return RtnValue { RtnType::ERR_UNBOUND, s.m_value + " is not defined" };
            }
            return RtnValue { RtnType::SUCC, "" };
        }
        case ExpType::List:
        {
            const List& s = static_cast<const List&>(exp);
            if (s.m_value.size() == 0)
            {
                return RtnValue { RtnType::ERR_ARGC, "unexpected empty list" };
            }

            std::shared_ptr<Expression> first = s.m_value[0];
            switch (first->m_type)
            {
                case ExpType::Integer:
                case ExpType::Float:
                    return RtnValue { RtnType::ERR_TYPE, "Unexpcted number in first element of list" };
                case ExpType::Symbol:
                case ExpType::List:
                {
                    std::shared_ptr<Expression> first_val;
                    RtnValue rtn = LI::Eval(*first, first_val, env);
                    if (rtn.m_type != RtnType::SUCC)
                    {
                        return rtn;
                    }
                    if (first_val->m_type != ExpType::Callable)
                    {
                        std::ostringstream ss;
                        ss << "Unexpcted type " << *first_val << " in first element of list";
                        return RtnValue { RtnType::ERR_TYPE, ss.str() };
                    }
                    first = first_val;
                }
                case ExpType::Callable: break;
            }

            // first is Callable
            std::vector<std::shared_ptr<Expression>> args(s.m_value.size() - 1, nullptr);
            RtnValue rtn = LI::Eval_seq(s.m_value, args, env, true);
            if (rtn.m_type != RtnType::SUCC)
            {
                return rtn;
            }
            const BuiltInFn& f = static_cast<const BuiltInFn&>(*first);
            return f.Call(args, result, env);
        }
        case ExpType::Callable:
        {
            const Callable& c = static_cast<const Callable&>(exp);
            result = std::make_shared<Callable>(c);
            return RtnValue { RtnType::SUCC, "" };
        }
    }
}

RtnValue
LI::Eval_seq(const std::vector<std::shared_ptr<Expression>>& exps, std::vector<std::shared_ptr<Expression>>& result, Environ& env, bool ignoreFirst)
{
    int diff = ignoreFirst ? 1 : 0;
    for (int i = 0; i < exps.size() - diff; ++i)
    {
        RtnValue rtn = LI::Eval(*exps[i + diff], result[i], env);
        if (rtn.m_type != RtnType::SUCC)
        {
            return rtn;
        }
    }
    return RtnValue { RtnType::SUCC, "" };
}

Add::Add()
    : BuiltInFn()
{
}

RtnValue
Add::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, Environ& env) const
{
    RtnValue rtn = CheckArgs(args, -1, 1);
    if (rtn.m_type != RtnType::SUCC)
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
