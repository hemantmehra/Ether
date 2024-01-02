#pragma once
#include <cstddef>
#include <string>
#include <memory>
#include <vector>

namespace Ether {
    
class Program;
class FunctionDeclaration;
class BlockStatement;
class ReturnStatement;
class Literal;

struct Position {
    size_t line { 0 };
    size_t column { 0 };
    size_t offset { 0 };
};

struct SourceRange {
    bool contains(Position const& position) const { return position.offset <= end.offset && position.offset >= start.offset; }

    Position start;
    Position end;
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void dump(int) const = 0;
    virtual std::string generate_c_code(int) const = 0;
    // std::string class_name() const;

    virtual bool is_program() const { return false; }
    virtual bool is_identifier() const { return false; }
    virtual bool is_function_declaration() const { return false; }
    virtual bool is_block_statement() const { return false; }
    virtual bool is_return_statement() const { return false; }
    virtual bool is_literal() const { return false; }
    virtual bool is_variable_declaration() const { return false; }

protected:
    ASTNode() {}

private:

};

class ScopeNode : public ASTNode {
public:
    template<typename T, typename... Args>
    T& append(Args&&... args)
    {
        auto child = std::make_shared<T>(std::forward<Args>(args)...);
        m_children.push_back(std::move(child));
        return static_cast<T&>(*m_children.back());
    }

    std::vector<std::shared_ptr<ASTNode>> children() const { return m_children; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;

protected:
    ScopeNode() {}

private:
    std::vector<std::shared_ptr<ASTNode>> m_children;
};

class Program : public ScopeNode
{
public:
    Program() {}

    virtual bool is_program() const override { return true; }

private:
};

class BlockStatement : public ScopeNode
{
public:
    BlockStatement() {}
    virtual bool is_block_statement() const override { return true; }

};

class Expression : public ASTNode
{
public:
    Expression() {}
    virtual void dump(int) const = 0;
    virtual std::string generate_c_code(int) const = 0;
};

class Identifier final : public Expression
{
public:
    Identifier(std::string name)
        : m_name(name) {}

    virtual bool is_identifier() const override { return true; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;
private:
    std::string m_name;
};

class VariableDeclaration : public ASTNode
{
public:
    VariableDeclaration(std::unique_ptr<Identifier> identifier, std::unique_ptr<Expression> expression)
        : m_identifier(std::move(identifier)), m_expression(std::move(expression))
    {}

    const Expression& expression() const { return *m_expression; }
    const Identifier& identifier() const { return *m_identifier; }
    virtual bool is_variable_declaration() const override { return true; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;

private:
    std::unique_ptr<Identifier> m_identifier;
    std::unique_ptr<Expression> m_expression;
};

class FunctionDeclaration : public ASTNode
{
public:
    FunctionDeclaration(std::string name, std::unique_ptr<ScopeNode> body)
        : m_name(name), m_body(std::move(body))
    {}

    const ScopeNode& body() const { return *m_body; }
    virtual bool is_function_declaration() const override { return true; }
    std::string name() const { return m_name; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;

private:
    std::string m_name;
    std::unique_ptr<ScopeNode> m_body;
};

class ReturnStatement : public ASTNode
{
public:
    ReturnStatement(std::unique_ptr<Expression> arg)
        : m_argument(std::move(arg))
    {}

    const Expression& argument() const { return *m_argument; }

    virtual bool is_return_statement() const override { return true; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;

private:
    std::unique_ptr<Expression> m_argument;
};

class Literal : public Expression
{
public:
    Literal(int a) : m_value(a) {}
    virtual bool is_literal() const override { return true; }
    virtual void dump(int) const override;
    virtual std::string generate_c_code(int) const override;
    int value() { return m_value; }

private:
    int m_value;
};

} // namespace Ether
