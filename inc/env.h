#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include "types.h"

namespace LI
{
    class Environ;

    class Environ
    {
        public:
            Environ(std::shared_ptr<Environ> p_parent);

            std::shared_ptr<Expression> Lookup(const std::string p_symbol);

            // Return false on override previous value.
            bool Add(const std::string p_symbol, std::shared_ptr<Expression> value);

        private:
            std::unordered_map<std::string, std::shared_ptr<Expression>> m_current;
            std::shared_ptr<Environ> m_parent;
    };
}
