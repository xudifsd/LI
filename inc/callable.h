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
        ERR_INTERNAL,
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

            static RtnValue CheckArgs(const std::vector<std::shared_ptr<Expression>>& args, int argc, int atLeast, int atMost);

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const = 0;

        private:
            CallableType m_type;
    };

    class BuiltInFn : public Callable {
        public:
            BuiltInFn();

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const = 0;
    };

    class BuiltInMacro : public Callable {
        public:
            BuiltInMacro();

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const = 0;
    };

    class Fn : public Callable
    {
        public:
            Fn(const std::vector<std::string>& p_parameters, const std::vector<std::shared_ptr<Expression>> p_body, std::shared_ptr<Environ> p_context);

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const override;

        private:
            std::vector<std::string> m_parameters;
            std::vector<std::shared_ptr<Expression>> m_body;
            std::shared_ptr<Environ> m_context;
    };

    std::string to_string(const RtnType& type);
    std::string to_string(const RtnValue& val);

    RtnValue Eval(const Expression& exp, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env);

    RtnValue Eval_seq(const std::vector<std::shared_ptr<Expression>>& exps, std::vector<std::shared_ptr<Expression>>& result, std::shared_ptr<Environ> env, bool ignoreFirst);
}
