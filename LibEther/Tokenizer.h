#pragma once
#include <string>
#include <optional>
#include <vector>
#include <LibEther/Assert.h>

#define LIST_TOKEN_TYPES \
    __LIST_TOKEN_TYPE(OpenParen)  /* () */ \
    __LIST_TOKEN_TYPE(CloseParen) \
    __LIST_TOKEN_TYPE(OpenBrace) /* {} */ \
    __LIST_TOKEN_TYPE(CloseBrace) \
    __LIST_TOKEN_TYPE(OpenBracket)  /* [] */ \
    __LIST_TOKEN_TYPE(CloseBracket) \
    __LIST_TOKEN_TYPE(Comma) \
    __LIST_TOKEN_TYPE(Semicolon) \
    __LIST_TOKEN_TYPE(Operator) \
    __LIST_TOKEN_TYPE(Constant) \
    __LIST_TOKEN_TYPE(String) \
    __LIST_TOKEN_TYPE(Identifier) \
    __LIST_TOKEN_TYPE(K_fn) \
    __LIST_TOKEN_TYPE(K_return) \

namespace Ether
{

enum class TokenType
{
#define __LIST_TOKEN_TYPE(x) x,
    LIST_TOKEN_TYPES
#undef __LIST_TOKEN_TYPE
};

struct Token
{
    TokenType token_type;
    int data_int;
    std::string data_str;

    Token(TokenType type) : token_type(type) {}
    std::string to_string();
    TokenType type() { return token_type; }
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
