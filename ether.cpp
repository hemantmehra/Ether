#include <iostream>
#include <LibEther/AST.h>

template<typename T, typename... Args>
std::unique_ptr<T> make(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main()
{
    using namespace Ether;
    auto program = make<Program>();

    auto block = make<BlockStatement>();
    block->append<VariableDeclaration>("x", make<Literal>(4));
    block->append<ReturnStatement>(make<Literal>(42));

    program->append<FunctionDeclaration>("main", std::move(block));

    program->dump(0);

    std::string c_code = program->generate_c_code(0);
    std::cout << "------------------------" << '\n';
    std::cout << c_code << '\n';
    return 0;
}
