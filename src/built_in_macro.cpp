#include <string>

#include "built_in_macro.h"

using namespace LI;

Lambda::Lambda()
    : BuiltInMacro()
{
}

RtnValue
Lambda::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 2, -1);
    if (rtn.IsError())
    {
        return rtn;
    }

    if (args[0]->m_type != ExpType::List)
    {
        return RtnValue
        {
            RtnType::ERR_TYPE,
            "expecting list as first element of `lambda` but got " + LI::to_string(*args[0]),
        };
    }

    const List& argList = static_cast<const List&>(*args[0]);
    std::vector<std::string> symbols(argList.m_value.size(), "");

    for (int i = 0; i < argList.m_value.size(); ++i)
    {
        if (argList.m_value[i]->m_type != ExpType::Symbol)
        {
            return RtnValue
            {
                RtnType::ERR_TYPE,
                "expecting list of symbol as lamba argument but got " + LI::to_string(*argList.m_value[i]),
            };
        }
        const Symbol& sym = static_cast<const Symbol&>(*argList.m_value[i]);
        symbols[i] = sym.m_value;
    }

    std::vector<std::shared_ptr<Expression>> body;
    body.reserve(args.size() - 1);
    for (int i = 1; i < args.size(); ++i)
    {
        body.push_back(args[i]);
    }

    result = std::make_shared<Fn>(symbols, body, env);
    return RtnValue { RtnType::SUCC, "" };
}


Let::Let()
    : BuiltInMacro()
{
}

RtnValue
Let::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 2, -1);
    if (rtn.IsError())
    {
        return rtn;
    }

    if (args[0]->m_type != ExpType::List)
    {
        return RtnValue
        {
            RtnType::ERR_TYPE,
            "expecting list as first element of `let` but got " + LI::to_string(*args[0]),
        };
    }

    const List& argList = static_cast<const List&>(*args[0]);
    std::shared_ptr<Environ> frame = std::make_shared<Environ>(env);

    for (int i = 0; i < argList.m_value.size(); ++i)
    {
        if (argList.m_value[i]->m_type != ExpType::List)
        {
            return RtnValue
            {
                RtnType::ERR_TYPE,
                "expecting list of list as first element of `let` but got list of " + LI::to_string(*args[0]),
            };
        }
        const List& pair = static_cast<const List&>(*argList.m_value[i]);
        if (pair.m_value.size() != 2)
        {
            return RtnValue
            {
                RtnType::ERR_ARGC,
                "expecting key value pair in `let` arg list but got " + std::to_string(pair.m_value.size()) + " elements",
            };
        }

        if (pair.m_value[0]->m_type != ExpType::Symbol)
        {
            return RtnValue
            {
                RtnType::ERR_TYPE,
                "expecting symbol as in `let` list but got " + LI::to_string(*args[0]),
            };
        }
        const Symbol& key = static_cast<const Symbol&>(*pair.m_value[0]);
        std::shared_ptr<Expression> val;
        rtn = LI::Eval(*pair.m_value[1], val, frame);
        if (rtn.IsError())
        {
            return rtn;
        }
        frame->Set(key.m_value, val);

    }

    for (int i = 1; i < args.size(); ++i)
    {
        rtn = LI::Eval(*args[i], result, frame);
        if (rtn.IsError())
        {
            return rtn;
        }
    }
    return rtn;
}

If::If()
    : BuiltInMacro()
{
}

RtnValue
If::IsTrue(const std::shared_ptr<Expression> cond, bool& result)
{
    switch (cond->m_type)
    {
        case ExpType::Symbol:
        case ExpType::Callable:
            return RtnValue
            {
                RtnType::ERR_TYPE,
                "expecting int/float/list as condition of `if` but got " + LI::to_string(*cond),
            };
        case ExpType::Integer:
        {
            const Integer& i = static_cast<const Integer&>(*cond);
            result = i.m_value != 0;
            break;
        }
        case ExpType::Float:
        {
            const Float& f = static_cast<const Float&>(*cond);
            result = f.m_value != 0;
            break;
        }
        case ExpType::List:
        {
            const List& l = static_cast<const List&>(*cond);
            result = l.m_value.size() != 0;
            break;
        }
        case ExpType::Nil:
        {
            result = false;
            break;
        }
    }
    return RtnValue { RtnType::SUCC, "" };
}

RtnValue
If::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 2, 3);
    if (rtn.IsError())
    {
        return rtn;
    }

    std::shared_ptr<Expression> cond;
    rtn = LI::Eval(*args[0], cond, env);
    if (rtn.IsError())
    {
        return rtn;
    }

    bool condition = false;
    rtn = If::IsTrue(cond, condition);
    if (rtn.IsError())
    {
        return rtn;
    }

    if (condition)
    {
        return LI::Eval(*args[1], result, env);
    }
    else
    {
        if (args.size() > 2)
        {
            return LI::Eval(*args[2], result, env);
        }
        else
        {
            result = std::make_shared<Nil>();
            return RtnValue { RtnType::SUCC, "" };
        }
    }
}
