#pragma once

#include <string>
#include <vector>
#include <memory>

#include "callable.h"

namespace LI
{
    class Add : BuiltInFn
    {
        public:
            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, Environ& env) const;
    };
}
