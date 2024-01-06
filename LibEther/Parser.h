#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <LibEther/AST.h>
#include <LibEther/Tokenizer.h>


namespace Ether
{

class ParserError
{
public:
    ParserError(std::string error) : m_error(error) {}
    std::string to_string() { return m_error; }

private:
    std::string m_error;
};

class Parser
{
public:
    Parser(std::vector<Token>);
    std::shared_ptr<Program> parse();
    void Error(std::string);
    std::vector<ParserError> get_errors() { return m_errors; }

private:
    std::shared_ptr<FunctionDeclaration> parse_function_declaration();
    std::unique_ptr<BlockStatement> parse_block_statement();
    std::shared_ptr<ASTNode> parse_statement();
    std::shared_ptr<VariableDeclaration> parse_variable_declaration();
    std::shared_ptr<ReturnStatement> parse_return_statement();
    // std::shared_ptr<Expression> parse_expression();

    std::optional<Token> seek_token(size_t);
    void consume_token();
    bool check_token(size_t, TokenType);

    size_t m_cursor { 0 };
    std::vector<Token> m_tokens;
    std::vector<ParserError> m_errors;
};

} // namespace Ether
