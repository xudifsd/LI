#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "exps.h"

namespace LI
{
    class Environ;

    class Environ
    {
        public:
            Environ(std::shared_ptr<Environ> p_parent);

            std::shared_ptr<Expression> Lookup(const std::string& p_symbol);

            // Return false on override previous value.
            bool Set(const std::string& p_symbol, std::shared_ptr<Expression> value);

        private:
            std::shared_ptr<Environ> m_parent;
            std::unordered_map<std::string, std::shared_ptr<Expression>> m_frame;

            friend std::ostream& operator<<(std::ostream& os, const LI::Environ& obj);
    };
}
