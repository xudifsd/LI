#include <vector>
#include <gtest/gtest.h>

#include "exps.h"
#include "test_utils.h"

using namespace LI;
using namespace LI_test;

TEST(testEval, correct)
{
    std::vector<std::shared_ptr<Expression>> result = eval("(+ 1 2 3)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 6);

    result = eval("(+ 1 2) (+ 1) (+ 1 2 3 4 5)");
    ASSERT_EQ(result.size(), 3);
    assert_int_exp(*result[0], 3);
    assert_int_exp(*result[1], 1);
    assert_int_exp(*result[2], 15);

    result = eval("(+ (+ 1 2) (+ (+ 1 2) 4))");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 10);

    result = eval("(+ (+ 1.1 2.2))");
    ASSERT_EQ(result.size(), 1);
    assert_float_exp(*result[0], 3.3, 0.00001);
}
