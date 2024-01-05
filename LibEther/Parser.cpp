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

// bool Parser::check_current_token(TokenType type)
// {
//     return seek_token(m_cursor).has_value() && seek_token(m_cursor).value().type() != type;
// }

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
        Error("Unexpected end of program. Missing function definition");
        return fn_decl;
    }

    if (seek_token(i).value().type() != TokenType::Identifier)
    {
        Error("Missing function name");
        return fn_decl;
    }
    
    consume_token(); // function name
    consume_token(); // (
    consume_token(); // )

    auto block = parse_block_statement();
    fn_decl = std::make_shared<FunctionDeclaration>(
        0,
        std::make_shared<Identifier>(seek_token(i).value().data_str),
        std::move(block)
    );
    
    return fn_decl;
}

std::unique_ptr<BlockStatement> Parser::parse_block_statement()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse_block_statement()" << '\n';
#endif
    size_t i = m_cursor;
    auto block = make<BlockStatement>();
    if (!seek_token(i).has_value()) {
        Error("Unexpected end of program. Missing function definition");
        return block;
    }

    if (seek_token(i).value().type() != TokenType::OpenBrace)
    {
        Error("Missing \'{\'.");
        return block;
    }

    consume_token(); // {

    while (seek_token(m_cursor).has_value() && seek_token(m_cursor).value().type() != TokenType::CloseBrace)
    {
        auto statement = parse_statement();
        block->append(statement);
    }

    if (seek_token(m_cursor).value().type() != TokenType::CloseBrace)
    {
        Error("Missing \'}\'.");
        return block;
    }

    consume_token(); // }
    return block;
}

std::shared_ptr<ASTNode> Parser::parse_statement()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse_statement()" << '\n';
#endif
    size_t i = m_cursor;
    std::shared_ptr<ASTNode> statement;
    if (!seek_token(i).has_value()) {
        Error("Unexpected end of program.");
        return statement;
    }

    if (seek_token(i).value().type() == TokenType::K_return)
    {
        statement = parse_return_statement();
    }

    else if (seek_token(i).value().type() == TokenType::K_let)
    {
        statement = parse_variable_declaration();
    }

    else
    {
        Error("Unexpected token at start of statement.");
    }


    return statement;
}

std::shared_ptr<VariableDeclaration> Parser::parse_variable_declaration()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse_variable_declaration()" << '\n';
#endif
    size_t i = m_cursor;
    auto var_decl = std::make_shared<VariableDeclaration>(0, std::make_shared<Identifier>("a"), std::make_shared<Literal>(42));
    consume_token(); // let
    consume_token(); // x
    consume_token(); // =
    consume_token(); // 1
    consume_token(); // ;
    return var_decl;
}

std::shared_ptr<ReturnStatement> Parser::parse_return_statement()
{
#ifdef PARSER_DEBUG
    std::cout << "Parser::parse_return_statement()" << '\n';
#endif
    size_t i = m_cursor;
    auto return_statement = std::make_shared<ReturnStatement>(make<Literal>(0));
    if (!seek_token(i).has_value()) {
        Error("Unexpected end of program.");
        return return_statement;
    }

    if (seek_token(i).value().type() != TokenType::K_return)
    {
        Error("Missing return keyword.");
        return return_statement;
    }

    consume_token();

    // if (seek_token())
    consume_token();
    return return_statement;
}

}
