#include "callable.h"

using namespace LI;

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
