#pragma once

#include <memory>
#include <vector>

#include "parser.h"
#include "token.h"
#include "exps.h"
#include "env.h"

namespace LI_test
{
    std::unique_ptr<std::istream> string_input(std::string s);

    void assert_token(const LI::Token& t, LI::TokenType type, size_t lineNum, size_t colNum, const std::string& lexeme);

    void assert_int_exp(const LI::ParseResult& p_exp, int p_val);
    void assert_float_exp(const LI::ParseResult& p_exp, float b, float epsilon);
    void assert_sym_exp(const LI::ParseResult& p_exp, const std::string& p_val);
    void assert_str_exp(const LI::ParseResult& p_exp, const std::string& p_val);
    void assert_eof_exp(const LI::ParseResult& result);

    void assert_int_exp(const LI::Expression& p_exp, int p_val);
    void assert_float_exp(const LI::Expression& p_exp, float b, float epsilon);
    void assert_sym_exp(const LI::Expression& p_exp, const std::string& p_val);
    void assert_str_exp(const LI::Expression& p_exp, const std::string& p_val);

    void assert_no_error_eval(const LI::Expression& exp, std::shared_ptr<LI::Expression>& result, std::shared_ptr<LI::Environ> env);

    std::vector<LI::ParseResult> parse(const std::string& input);

    std::vector<std::shared_ptr<LI::Expression>> eval(const std::string& input);

    std::vector<std::shared_ptr<LI::Expression>> eval(const std::string& input, std::shared_ptr<LI::Environ> base);

    std::vector<LI::Token> tokenize(const std::string& input);

    void RunAndReport(const std::string& input, int times);

    void RunPerf();
}
