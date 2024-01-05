#include <iostream>
#include <LibEther/Parser.h>

#define PARSER_DEBUG

namespace Ether
{

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(tokens)
{}

void Parser::Error(std::string err)
{
    m_errors.push_back(ParserError(err));
}

std::optional<Token> Parser::seek_token(size_t i)
{
    if (i < m_tokens.size()) return m_tokens.at(i);
    return {};
}

void Parser::consume_token()
{
#ifdef PARSER_DEBUG
    std::cout << "Consume: " << seek_token(m_cursor).value().to_string() << '\n';
#endif
    m_cursor++;
}

template <typename T, typename... Args>
std::unique_ptr<T> make(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

std::shared_ptr<Program> Parser::parse()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse()" << '\n';
#endif
    if (seek_token(m_cursor).has_value())
    {
        if (seek_token(m_cursor).value().type() == TokenType::K_fn) {
            consume_token();
            auto program = std::make_unique<Program>();
            auto fn_decl = parse_function_declaration();
            program->append(fn_decl);
            // program->append<FunctionDeclaration>(
            //     0,
            //     std::make_shared<Identifier>(fn_decl->name()),
            //     // make<BlockStatement>()
            //     std::move(fn_decl->body())
            // );
            return program;
        }
        return {};
    }
    return {};
}

std::shared_ptr<FunctionDeclaration> Parser::parse_function_declaration()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse_function_declaration()" << '\n';
#endif
    size_t i = m_cursor;
    auto fn_decl = std::make_shared<FunctionDeclaration>();
    if (!seek_token(i).has_value()) {
        Error("Missing function definition");
        return fn_decl;
    }

    if (seek_token(i).value().type() != TokenType::Identifier)
    {
        Error("Missing function name");
        return fn_decl;
    }
    
    consume_token();
    consume_token();
    consume_token();

    auto block = parse_block_statement();
    auto fn_block = make<BlockStatement>();
    fn_decl = std::make_shared<FunctionDeclaration>(
        0,
        std::make_shared<Identifier>(seek_token(i).value().data_str),
        std::move(block)
    );
    
    return fn_decl;
}

std::unique_ptr<BlockStatement> Parser::parse_block_statement()
{
    size_t i = m_cursor;
    if (seek_token(i).has_value() && seek_token(i+1).has_value()) {
        if (seek_token(i).value().type() == TokenType::OpenBrace) {
            consume_token();
            auto return_statement = parse_return_statement();
            consume_token();
            auto block = make<BlockStatement>();
            block->append(return_statement);
            return block;
        }
    }
    return {};
}

// std::shared_ptr<VariableDeclaration> Parser::parse_variable_declaration()
// {}

std::shared_ptr<ReturnStatement> Parser::parse_return_statement()
{
    size_t i = m_cursor;
    if (seek_token(i).has_value() && seek_token(i+1).has_value()) {
        if (seek_token(i).value().type() == TokenType::K_return &&
            seek_token(i+1).value().type() == TokenType::Semicolon
        ) {
            consume_token();
            consume_token();
            auto return_statement = std::make_shared<ReturnStatement>(make<Literal>(0));
            return return_statement;
        }
    }
    return {};
}

}
