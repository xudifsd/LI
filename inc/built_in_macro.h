#pragma once

#include <string>
#include <vector>
#include <memory>

#include "callable.h"

namespace LI
{
    class Lambda : public BuiltInMacro
    {
        public:
            Lambda();

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, std::shared_ptr<Environ> env) const override;
    };
}
