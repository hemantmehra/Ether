#include <iostream>
#include <LibEther/AST.h>

template <typename T, typename... Args>
std::unique_ptr<T> make(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main()
{
    using namespace Ether;
    auto program = make<Program>();

    auto struct_block = make<BlockStatement>();
    auto func_block = make<BlockStatement>();

    struct_block->append<VariableDeclaration>(
        std::make_shared<Identifier>("x") //, make<Literal>(4)
    );

    func_block->append<VariableDeclaration>(
        std::make_shared<Identifier>("x") // , make<Literal>(4)
    );
    func_block->append<ReturnStatement>(std::make_shared<Identifier>("x"));

    program->append<StructDeclaration>(std::make_shared<Identifier>("Vector"),
                                        std::move(struct_block));
    program->append<FunctionDeclaration>(std::make_shared<Identifier>("main"),
                                        std::move(func_block));

    program->dump(0);

    std::string c_code = program->generate_c_code(0);
    std::cout << "------------------------" << '\n';
    std::cout << c_code << '\n';
    return 0;
}
