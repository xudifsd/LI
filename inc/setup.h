#pragma once

#include <string>
#include <memory>

#include "env.h"
#include "built_in_fn.h"
#include "built_in_macro.h"

namespace LI
{
    std::shared_ptr<Environ> setup_base();
}
