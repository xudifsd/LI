#include "setup.h"

using namespace LI;

std::shared_ptr<Environ>
LI::setup_base()
{
    std::shared_ptr<Environ> result = std::make_shared<Environ>(nullptr);

    result->Set("+", std::make_shared<Add>());
    result->Set("lambda", std::make_shared<Lambda>());

    return result;
}
