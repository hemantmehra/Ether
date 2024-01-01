#include <iostream>
#include <LibEther/AST.h>

template<typename T, typename... Args>
std::unique_ptr<T> make(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main()
{
    auto program = make<Ether::Program>();

    auto block = make<Ether::BlockStatement>();
    block->append<Ether::ReturnStatement>(make<Ether::Literal>(42));

    program->append<Ether::FunctionDeclaration>("main", std::move(block));

    program->dump(0);

    std::string c_code = program->generate_c_code(0);
    std::cout << c_code << '\n';
    return 0;
}
