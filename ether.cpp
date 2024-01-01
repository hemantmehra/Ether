#include <iostream>
#include <LibEther/AST.h>

template<typename T, typename... Args>
std::shared_ptr<T> make(Args... args) {
    return std::make_shared<T>(args...);
}

int main()
{
    auto program = make<Ether::Program>();

    auto block = make<Ether::BlockStatement>();
    block->append<Ether::ReturnStatement>(make<Ether::Literal>(42));

    program->append<Ether::FunctionDeclaration>("main", block);

    program->dump(0);
    return 0;
}
