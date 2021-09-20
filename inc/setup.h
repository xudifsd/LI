#pragma once

#include <string>
#include <memory>

#include "env.h"
#include "built_in_fn.h"

namespace LI
{
    std::shared_ptr<Environ> setup_base();
}
