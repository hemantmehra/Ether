#include <ctype.h>
#include <LibEther/Tokenizer.h>

#define TOKENIZER_DEBUG 1

namespace Ether
{

bool is_word_char(char ch)
{
    if (isalnum(ch) || ch == '_') return true;
    return false;
}

Tokenizer::Tokenizer(std::string input)
    : m_input(input)
    {}

std::optional<Token> Tokenizer::next_token()
{
    auto word = next_word();
    if (!word.has_value()) return {};

    return convert_to_token(word.value());
}

std::optional<std::string> Tokenizer::next_word()
{
    std::string buffer = "";

    if (m_cursor >= m_input.length()) return {};

    while (m_cursor < m_input.length()) {
        auto curr_char = seek_char(m_cursor);

        if (!curr_char.has_value()) break;
        char ch = curr_char.value();

        if (isspace(ch) && buffer.length()) {
            consume_char();
            return buffer;
        }

        if (ispunct(ch) && buffer.length()) {
            return buffer;
        }

        if (isalnum(ch)) {
            consume_char();
            buffer += ch;
        }

        if (ispunct(ch)) {
            consume_char();
            buffer += ch;
            return buffer;
        }

        if (isspace(ch)) consume_char();
    }
    return buffer;
}

std::optional<char> Tokenizer::seek_char(size_t i)
{
    if (i < m_input.length()) return m_input.at(i);
    return {};
}

void Tokenizer::consume_char()
{
    m_cursor++;
}

bool is_numeric(std::string word)
{
    return false;
}

Token Tokenizer::convert_to_token(std::string word)
{
    if (word == "(") return Token(TokenType::OpenParen);
    if (word == ")") return Token(TokenType::CloseParen);
    if (word == "{") return Token(TokenType::OpenBrace);
    if (word == "}") return Token(TokenType::CloseBrace);
    if (word == "[") return Token(TokenType::OpenBracket);
    if (word == "]") return Token(TokenType::CloseBracket);

    if (word == ",") return Token(TokenType::Comma);
    if (word == ";") return Token(TokenType::Semicolon);

    if (word == "fn") return Token(TokenType::K_fn);
    if (word == "return") return Token(TokenType::K_return); 

    if (is_numeric(word)) {
        auto token = Token(TokenType::Constant);
        return token;
    }

    else {
        auto token = Token(TokenType::Identifier);
        token.data_str = word;
        return token;
    }
}

std::string Token::to_string()
{
    std::string s = "";

    switch (token_type)
    {
#define __LIST_TOKEN_TYPE(x) \
    case TokenType::x:       \
        s = #x;             \
        break;

        LIST_TOKEN_TYPES
#undef __LIST_TOKEN_TYPE
    }

    return s;
}

}
