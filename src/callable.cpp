#include <sstream>

#include "callable.h"

using namespace LI;

bool
RtnValue::IsError()
{
    return m_type != RtnType::SUCC;
}

Callable::Callable(CallableType p_type)
    : Expression(ExpType::Callable),
      m_type(p_type)
{
}

CallableType
Callable::GetType() const
{
    return m_type;
}

RtnValue
Callable::CheckArgs(const std::vector<std::shared_ptr<Expression>>& args, int argc, int atLeast)
{
    if (argc != -1 && args.size() != argc)
    {
        return RtnValue
        {
            RtnType::ERR_ARGC,
            "expecting " + std::to_string(argc) + " args but got " + std::to_string(args.size())
        };
    }

    if (args.size() < atLeast)
    {
        return RtnValue
        {
            RtnType::ERR_ARGC,
            "expecting at least " + std::to_string(atLeast) + " args but got " + std::to_string(args.size())
        };
    }
    return RtnValue { RtnType::SUCC, "" };
}

BuiltInFn::BuiltInFn()
    : Callable(CallableType::BUILT_IN_FN)
{
}

BuiltInFn::~BuiltInFn()
{
}

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
