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
Callable::CheckArgs(const std::vector<std::shared_ptr<Expression>>& args, int argc, int atLeast, int atMost)
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

    if (atMost != -1 && args.size() > atMost)
    {
        return RtnValue
        {
            RtnType::ERR_ARGC,
            "expecting at most " + std::to_string(atMost) + " args but got " + std::to_string(args.size())
        };
    }
    return RtnValue { RtnType::SUCC, "" };
}

BuiltInFn::BuiltInFn()
    : Callable(CallableType::BUILT_IN_FN)
{
}

BuiltInMacro::BuiltInMacro()
    : Callable(CallableType::BUILT_IN_MACRO)
{
}

Fn::Fn(const std::vector<std::string>& p_parameters, const std::vector<std::shared_ptr<Expression>> p_body, std::shared_ptr<Environ> p_context)
    : Callable(CallableType::FN),
      m_parameters(p_parameters),
      m_body(p_body),
      m_context(p_context)
{
}

RtnValue
Fn::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    if (args.size() != m_parameters.size())
    {
        return RtnValue
        {
            RtnType::ERR_ARGC,
            "expecting " + std::to_string(m_parameters.size()) + " args but got " + std::to_string(args.size())
        };
    }
    std::shared_ptr<Environ> frame = std::make_shared<Environ>(m_context);
    for (int i = 0; i < args.size(); ++i)
    {
        frame->Set(m_parameters[i], args[i]);
    }

    RtnValue v;
    for (int i = 0; i < m_body.size(); ++i)
    {
        v = LI::Eval(*m_body[i], result, frame);
        if (v.IsError())
        {
            return v;
        }
    }
    return v;
}

std::string
LI::to_string(const RtnType& type)
{
    switch (type)
    {
        case RtnType::SUCC: return "SUCC";
        case RtnType::ERR_ARGC: return "ERR_ARGC";
        case RtnType::ERR_TYPE: return "ERR_TYPE";
        case RtnType::ERR_MATH: return "ERR_MATH";
        case RtnType::ERR_UNBOUND: return "ERR_UNBOUND";
        case RtnType::ERR_INTERNAL: return "ERR_INTERNAL";
    }
}

std::string
LI::to_string(const RtnValue& val)
{
    return LI::to_string(val.m_type) + ":" + val.m_msg;
}

RtnValue
LI::Eval(const Expression& exp, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env)
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
        case ExpType::Nil:
        {
            result = std::make_shared<Nil>();
            return RtnValue { RtnType::SUCC, "" };
        }
        case ExpType::Symbol:
        {
            const Symbol& s = static_cast<const Symbol&>(exp);
            result = env->Lookup(s.m_value);
            if (result == nullptr)
            {
                return RtnValue { RtnType::ERR_UNBOUND, s.m_value + " is not defined" };
            }
            return RtnValue { RtnType::SUCC, "" };
        }
        case ExpType::List:
        {
            const List& l = static_cast<const List&>(exp);
            if (l.m_value.size() == 0)
            {
                return RtnValue { RtnType::ERR_ARGC, "unexpected empty list" };
            }

            std::shared_ptr<Expression> first = l.m_value[0];
            switch (first->m_type)
            {
                case ExpType::Integer:
                case ExpType::Float:
                case ExpType::Nil:
                case ExpType::String:
                    return RtnValue { RtnType::ERR_TYPE, "Unexpcted " + LI::to_string(*first) + " in first element of list" };
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
                        auto s = "Unexpcted type " + LI::to_string(*first_val) + " in first element of list";
                        return RtnValue { RtnType::ERR_TYPE, s };
                    }
                    first = first_val;
                }
                case ExpType::Callable: break;
            }

            // first is Callable
            const Callable& c = static_cast<const Callable&>(*first);

            switch (c.GetType())
            {
                case CallableType::BUILT_IN_FN:
                case CallableType::FN:
                {
                    std::vector<std::shared_ptr<Expression>> args(l.m_value.size() - 1, nullptr);
                    RtnValue rtn = LI::Eval_seq(l.m_value, args, env, true);
                    if (rtn.m_type != RtnType::SUCC)
                    {
                        return rtn;
                    }
                    return c.Call(args, result, env);
                }
                case CallableType::BUILT_IN_MACRO:
                {
                    std::vector<std::shared_ptr<Expression>> args(l.m_value.size() - 1, nullptr);
                    const BuiltInMacro& f = static_cast<const BuiltInMacro&>(c);
                    for (int i = 1; i < l.m_value.size(); ++i)
                    {
                        args[i - 1] = l.m_value[i];
                    }
                    return f.Call(args, result, env);
                }
            }
        }
        case ExpType::Callable: return RtnValue { RtnType::ERR_INTERNAL, "unexpected callable" };
        case ExpType::String:
        {
            const String& s = static_cast<const String&>(exp);
            result = std::make_shared<String>(s.m_value);
            return RtnValue { RtnType::SUCC, "" };
        }
    }
}

RtnValue
LI::Eval_seq(const std::vector<std::shared_ptr<Expression>>& exps, std::vector<std::shared_ptr<Expression>>& result, std::shared_ptr<Environ> env, bool ignoreFirst)
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
