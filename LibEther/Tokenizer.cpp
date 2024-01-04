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

}
