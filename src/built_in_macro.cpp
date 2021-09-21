#include <sstream>

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
        ss << "expecting list but got " << *args[0];
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
