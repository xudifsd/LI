#include <sstream>
#include <iostream>

#include "built_in_macro.h"

using namespace LI;

Lambda::Lambda()
    : BuiltInMacro()
{
}

RtnValue
Lambda::Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const
{
    RtnValue rtn = CheckArgs(args, -1, 2);
    if (rtn.IsError())
    {
        return rtn;
    }

    if (args[0]->m_type != ExpType::List)
    {
        std::ostringstream ss;
        ss << "expecting list as first element of `lambda` but got " << *args[0];
        return RtnValue
        {
            RtnType::ERR_TYPE,
                ss.str(),
        };
    }

    const List& argList = static_cast<const List&>(*args[0]);
    std::vector<std::string> symbols(argList.m_value.size(), "");

    for (int i = 0; i < argList.m_value.size(); ++i)
    {
        if (argList.m_value[i]->m_type != ExpType::Symbol)
        {
            std::ostringstream ss;
            ss << "expecting list of symbol as lamba argument but got " << *argList.m_value[i];
            return RtnValue
            {
                RtnType::ERR_TYPE,
                    ss.str(),
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
    RtnValue rtn = CheckArgs(args, -1, 2);
    if (rtn.IsError())
    {
        return rtn;
    }

    if (args[0]->m_type != ExpType::List)
    {
        std::ostringstream ss;
        ss << "expecting list as first element of `let` but got " << *args[0];
        return RtnValue
        {
            RtnType::ERR_TYPE,
            ss.str(),
        };
    }

    const List& argList = static_cast<const List&>(*args[0]);
    std::shared_ptr<Environ> frame = std::make_shared<Environ>(env);

    for (int i = 0; i < argList.m_value.size(); ++i)
    {
        if (argList.m_value[i]->m_type != ExpType::List)
        {
            std::ostringstream ss;
            ss << "expecting list of list as first element of `let` but got list of " << *args[0];
            return RtnValue
            {
                RtnType::ERR_TYPE,
                ss.str(),
            };
        }
        const List& pair = static_cast<const List&>(*argList.m_value[i]);
        if (pair.m_value.size() != 2)
        {
            std::ostringstream ss;
            ss << "expecting key value pair in `let` arg list but got " << pair.m_value.size() << " elements";
            return RtnValue
            {
                RtnType::ERR_ARGC,
                ss.str(),
            };
        }

        if (pair.m_value[0]->m_type != ExpType::Symbol)
        {
            std::ostringstream ss;
            ss << "expecting symbol as in `let` list but got " << *args[0];
            return RtnValue
            {
                RtnType::ERR_TYPE,
                ss.str(),
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
