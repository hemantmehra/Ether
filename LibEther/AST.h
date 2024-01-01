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
    // std::string class_name() const;

    virtual bool is_program() const { return false; }
    virtual bool is_function_declaration() const { return false; }
    virtual bool is_block_statement() const { return false; }
    virtual bool is_return_statement() const { return false; }
    virtual bool is_literal() const { return false; }

protected:
    ASTNode() {}

private:

};

class ScopeNode : public ASTNode {
public:
    template<typename T, typename... Args>
    void append(Args&&... args)
    {
        auto child = std::make_shared<T>(std::forward<Args>(args)...);
        m_children.push_back(child);
        // return m_children.back();
    }

    std::vector<std::shared_ptr<ASTNode>> children() const { return m_children; }
    virtual void dump(int) const override;

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

private:
    std::string m_name;
    std::unique_ptr<ScopeNode> m_body;
};

class ReturnStatement : public ASTNode
{
public:
    ReturnStatement(std::unique_ptr<ASTNode> arg)
        : m_argument(std::move(arg))
    {}

    const ASTNode& argument() const { return *m_argument; }

    virtual bool is_return_statement() const override { return true; }
    virtual void dump(int) const override;

private:
    std::unique_ptr<ASTNode> m_argument;
};

class Literal : public ASTNode
{
public:
    Literal(int a) : m_value(a) {}
    virtual bool is_literal() const override { return true; }
    virtual void dump(int) const override;
    int value() { return m_value; }

private:
    int m_value;
};

} // namespace Ether
