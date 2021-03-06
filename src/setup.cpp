#include "setup.h"

using namespace LI;

std::shared_ptr<Environ>
LI::setup_base()
{
    std::shared_ptr<Environ> result = std::make_shared<Environ>(nullptr);

    result->Set("+", std::make_shared<Add>());
    result->Set("lambda", std::make_shared<Lambda>());
    result->Set("let", std::make_shared<Let>());
    result->Set("if", std::make_shared<If>());

    return result;
}
