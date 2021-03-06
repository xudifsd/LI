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

TEST(testBuiltIn, lambdaClosure)
{
    std::vector<std::shared_ptr<Expression>> result = eval("((lambda (x) ((lambda (y) (+ x y)) (+ x 1))) 5)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 11);
}

TEST(testBuiltIn, letClosure)
{
    std::vector<std::shared_ptr<Expression>> result = eval("((let ((x 1)) (lambda (y) (+ x y))) 5)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 6);
}

TEST(testBuiltIn, ifWithTwo)
{
    std::vector<std::shared_ptr<Expression>> result = eval("(if 1 2)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 2);

    result = eval("(if 0 2)");
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0]->m_type, ExpType::Nil);
}

TEST(testBuiltIn, ifThree)
{
    std::vector<std::shared_ptr<Expression>> result = eval("(if 1 2 3)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 2);

    result = eval("(if 0 2 3)");
    ASSERT_EQ(result.size(), 1);
    assert_int_exp(*result[0], 3);
}
