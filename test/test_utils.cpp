#include "test_utils.h"
#include "token.h"
#include "exps.h"

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
