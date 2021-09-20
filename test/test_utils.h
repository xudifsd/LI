#pragma once

#include <memory>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <gtest/gtest.h>

#include "parser.h"
#include "token.h"
#include "exps.h"

namespace LI_test
{
    std::unique_ptr<std::istream> string_input(std::string s);

    void assert_token(const LI::Token& t, LI::TokenType type, size_t lineNum, size_t colNum, const std::string& lexeme);

    void assert_int_exp(const LI::Expression& p_exp, int p_val);

    void assert_float_exp(const LI::Expression& p_exp, float b, float epsilon);

    void assert_sym_exp(const LI::Expression& p_exp, const std::string& p_val);
}
