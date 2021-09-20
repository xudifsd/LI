#pragma once

#include <string>
#include <vector>
#include <memory>

#include "env.h"
#include "exps.h"

namespace LI
{
    enum class CallableType
    {
        BUILT_IN_FN,
        BUILT_IN_MACRO,
        FN,
    };

    enum class RtnType
    {
        SUCC,
        ERR_ARGC,
        ERR_TYPE,
        ERR_MATH,
        ERR_UNBOUND,
    };

    struct RtnValue
    {
        RtnType m_type;
        std::string m_msg;

        bool IsError();
    };

    class Callable : public Expression
    {
        public:
            Callable(CallableType p_type);

            CallableType GetType() const;

            static RtnValue CheckArgs(const std::vector<std::shared_ptr<Expression>>& args, int argc, int atLeast);

        private:
            CallableType m_type;
    };

    class BuiltInFn : public Callable {
        public:
            BuiltInFn();

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, Environ& env) const = 0;
            virtual ~BuiltInFn();
    };

    RtnValue Eval(const Expression& exp, std::shared_ptr<Expression>& result, Environ& env);

    RtnValue Eval_seq(const std::vector<std::shared_ptr<Expression>>& exps, std::vector<std::shared_ptr<Expression>>& result, Environ& env, bool ignoreFirst);
}
