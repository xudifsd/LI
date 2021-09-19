#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <math.h>

#include "parser.h"
#include "token.h"
#include "types.h"

using namespace LI;


std::unique_ptr<std::istream> string_input(std::string s)
{
    return std::make_unique<std::istringstream>(std::istringstream(s));
}


void assert_int_exp(const Expression& p_exp, int p_val)
{
    std::cout << p_exp << std::endl;
    assert(p_exp.m_type == ExpType::Integer);
    const Integer& i = static_cast<const Integer&>(p_exp);
    assert(i.m_value == p_val);
}

void assert_float_exp(const Expression& p_exp, float b, float epsilon)
{
    std::cout << p_exp << std::endl;
    assert(p_exp.m_type == ExpType::Float);
    const Float& i = static_cast<const Float&>(p_exp);
    float a = i.m_value;
    assert(fabs(a - b) <= epsilon * std::max(fabs(a), fabs(b)));
}

void assert_sym_exp(const Expression& p_exp, const std::string& p_val)
{
    std::cout << p_exp << std::endl;
    assert(p_exp.m_type == ExpType::Symbol);
    const Symbol& i = static_cast<const Symbol&>(p_exp);
    assert(i.m_value == p_val);
}

void correct()
{
    Parser parser("<input>", string_input("Hello (+ (/ 2.123 .3 ) 122)\n abd + .2"));

    std::vector<std::shared_ptr<Expression>> exps;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        exps.push_back(e.m_exp);
        e = parser.NextExp();
    }
    exps.push_back(e.m_exp);

    assert(exps.size() == 6);
    assert_sym_exp(*exps[0], "Hello");

    assert(exps[1]->m_type == ExpType::List);
    const List& list = static_cast<const List&>(*exps[1]);
    assert(list.m_value.size() == 3);

    assert_sym_exp(*list.m_value[0], "+");

    assert(list.m_value[1]->m_type == ExpType::List);
    const List& sublist = static_cast<const List&>(*list.m_value[1]);
    assert(sublist.m_value.size() == 3);
    assert_sym_exp(*sublist.m_value[0], "/");
    assert_float_exp(*sublist.m_value[1], 2.123, 0.00001);
    assert_float_exp(*sublist.m_value[2], 0.3, 0.00001);

    assert_int_exp(*list.m_value[2], 122);

    assert_sym_exp(*exps[2], "abd");
    assert_sym_exp(*exps[3], "+");
    assert_float_exp(*exps[4], 0.2, 0.00001);

    assert(exps[5] == nullptr);
}

void incorrect1()
{
    Parser parser("<input>", string_input(".1.21"));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    assert(results.size() == 1);
    assert(results[0].m_isError);
    assert(results[0].m_token.m_type == TokenType::ERROR);
    assert(results[0].m_token.m_lexeme == ".1.");
}

void incorrect2()
{
    Parser parser("<input>", string_input("(abd"));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    assert(results.size() == 1);
    assert(results[0].m_isError);
    assert(results[0].m_token.m_type == TokenType::TEOF);
}

void incorrect3()
{
    Parser parser("<input>", string_input("(.1."));

    std::vector<ParseResult> results;
    ParseResult e = parser.NextExp();
    while (!e.m_isError && e.m_token.m_type != TokenType::TEOF)
    {
        results.push_back(e);
        e = parser.NextExp();
    }
    results.push_back(e);

    assert(results.size() == 1);
    assert(results[0].m_isError);
    assert(results[0].m_token.m_type == TokenType::ERROR);
    assert(results[0].m_token.m_lexeme == ".1.");
}

int main()
{
    correct();
    incorrect1();
    incorrect2();
    incorrect3();
    return 0;
}
