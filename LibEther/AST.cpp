#include <iostream>
#include <LibEther/AST.h>

namespace Ether {

void print_indent(int indent)
{
    for(int i=0; i<indent * 2; i++)
        putchar(' ');
}

void ScopeNode::dump(int indent) const
{
    auto children = m_children;
    for(auto&& i: children) {
        i->dump(indent);
    }
}

void FunctionDeclaration::dump(int indent) const
{
    print_indent(indent);
    std::cout << "FunctionDeclaration: " << name() << '\n';
    body().dump(indent + 1);
}

void ReturnStatement::dump(int indent) const
{
    print_indent(indent);
    std::cout << "ReturnStatement" << '\n';
    argument().dump(indent+1);
}

void Literal::dump(int indent) const
{
    print_indent(indent);
    std::cout << "Literal: " << m_value << '\n';
}

}
