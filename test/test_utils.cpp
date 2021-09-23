#include <gtest/gtest.h>
#include <math.h>
#include <chrono>

#include "test_utils.h"
#include "token.h"
#include "exps.h"
#include "setup.h"
#include "parser.h"
#include "built_in_fn.h"

using namespace LI;

std::unique_ptr<std::istream> LI_test::string_input(std::string s)
{
    return std::make_unique<std::istringstream>(std::istringstream(s));
}

void LI_test::assert_token(const LI::Token& t, LI::TokenType type, size_t lineNum, size_t colNum, const std::string& lexeme)
{
    ASSERT_EQ(t.m_type, type);
    ASSERT_EQ(t.m_lineNum, lineNum);
    ASSERT_EQ(t.m_colNum, colNum);
    ASSERT_EQ(t.m_lexeme, lexeme);
}

void LI_test::assert_int_exp(const Expression& p_exp, int p_val)
{
    ASSERT_EQ(p_exp.m_type, ExpType::Integer);
    const Integer& i = static_cast<const Integer&>(p_exp);
    ASSERT_EQ(i.m_value, p_val);
}

void LI_test::assert_float_exp(const Expression& p_exp, float b, float epsilon)
{
    ASSERT_EQ(p_exp.m_type, ExpType::Float);
    const Float& i = static_cast<const Float&>(p_exp);
    float a = i.m_value;
    ASSERT_TRUE(fabs(a - b) <= epsilon * std::max(fabs(a), fabs(b)));
}

void LI_test::assert_sym_exp(const Expression& p_exp, const std::string& p_val)
{
    ASSERT_EQ(p_exp.m_type, ExpType::Symbol);
    const Symbol& i = static_cast<const Symbol&>(p_exp);
    ASSERT_EQ(i.m_value, p_val);
}

std::vector<std::shared_ptr<Expression>> LI_test::eval(const std::string& input)
{
    std::shared_ptr<Environ> base = setup_base();
    return LI_test::eval(input, base);
}

std::vector<std::shared_ptr<Expression>> LI_test::eval(const std::string& input, std::shared_ptr<Environ> base)
{
    Parser parser("<input>", LI_test::string_input(input));

    std::vector<std::shared_ptr<Expression>> result;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        std::shared_ptr<Expression> r;
        RtnValue v = Eval(*e.m_exp, r, base);

        result.push_back(r);
        e = parser.NextExp();
    }
    return result;
}

void LI_test::RunAndReport(const std::string& input, int times)
{
    std::shared_ptr<Environ> base = setup_base();
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < times; ++i)
    {
        LI_test::eval(input, base);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "eval " << input << " " << times << " times spend " << duration.count() << "ms" << std::endl;
}

void LI_test::RunPerf()
{
    static const int times = 1000;

    RunAndReport("(+ 1 2)", times);
    RunAndReport("((lambda (x) (+ x x)) 5)", times);
}
