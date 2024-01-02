#pragma once

#include <LibEther/AST.h>

namespace Ether {

class Function {
public:
    Function(std::string name, std::shared_ptr<ScopeNode> body);

    const std::string name() const { return m_name; }
    const std::shared_ptr<ScopeNode> body() const { return m_body; }

private:
    std::string m_name;
    std::shared_ptr<ScopeNode> m_body;
};

}
