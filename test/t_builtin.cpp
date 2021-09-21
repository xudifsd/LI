#include <vector>
#include <gtest/gtest.h>

#include "exps.h"
#include "test_utils.h"

using namespace LI;
using namespace LI_test;

TEST(testBuiltIn, correct)
{
    std::vector<std::shared_ptr<Expression>> result = eval("(+ ((lambda (x) (+ x x)) 5) 2)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 12);
}

TEST(testBuiltIn, closure)
{
    std::vector<std::shared_ptr<Expression>> result = eval("((lambda (x) ((lambda (y) (+ x y)) (+ x 1))) 5)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 11);
}
