#include <iostream>
#include <LibEther/AST.h>

namespace Ether {

    void print_indent(int indent)
    {
        for(int i=0; i<indent; i++)
            putchar(' ');
    }

    void ScopeNode::dump(int indent) const
    {
        std::cout << "ScopeNode\n";
    }

    void FunctionDeclaration::dump(int indent) const
    {
        
    }

    void ReturnStatement::dump(int indent) const
    {
        
    }

    void Literal::dump(int indent) const
    {
        
    }


}