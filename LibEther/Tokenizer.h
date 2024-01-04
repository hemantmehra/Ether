#pragma once
#include <string>
#include <optional>
#include <vector>

namespace Ether
{

enum class TokenType
{
    // ()
    OpenParen,
    CloseParen,
    // {}
    OpenBrace,
    CloseBrace,
    // []
    OpenBracket ,
    CloseBracket,

    Comma,
    Semicolon,
    Operator,
    Keyword,
    Constants,
    String
};

struct Token
{
    TokenType token_type;
    int data_int;
    std::string data_str;
};

class Tokenizer
{
public:
    Tokenizer(std::string);
    std::optional<Token> next_token();
    Token convert_to_token(std::string);

private:
    std::optional<std::string> next_word();
    std::optional<char> seek_char(size_t);
    void consume_char();

    std::string m_input;
    size_t m_cursor{ 0 };
};

}
