#pragma once

#include <string>
#include <vector>
#include <memory>

#include "callable.h"

namespace LI
{
    class Add : public BuiltInFn
    {
        public:
            Add();

            virtual RtnValue Call(const std::vector<std::shared_ptr<Expression>>& args, std::shared_ptr<Expression>& result, Environ& env) const override;
    };
}
