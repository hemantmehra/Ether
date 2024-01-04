#include <fstream>
#include <iostream>
#include <LibEther/AST.h>
#include <LibEther/DataTypes.h>
#include <LibEther/Tokenizer.h>

template <typename T, typename... Args>
std::unique_ptr<T> make(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main()
{
    using namespace Ether;
    auto datatype_list = DataTypeList::get_instance();
    datatype_list->add("struct Vector");

    auto program = make<Program>();

    auto struct_block = make<BlockStatement>();
    auto func_block = make<BlockStatement>();

    struct_block->append<VariableDeclaration>(
        0, std::make_shared<Identifier>("x"), nullptr
    );
    struct_block->append<VariableDeclaration>(
        0, std::make_shared<Identifier>("y"), nullptr
    );

    func_block->append<VariableDeclaration>(
        0, std::make_shared<Identifier>("a"), std::make_shared<Literal>(42)
    );
    func_block->append<VariableDeclaration>(
        1, std::make_shared<Identifier>("v1"), nullptr
    );
    func_block->append<ReturnStatement>(std::make_shared<Identifier>("a"));

    program->append<StructDeclaration>(std::make_shared<Identifier>("Vector"),
                                        std::move(struct_block));
    program->append<FunctionDeclaration>(0,
                                        std::make_shared<Identifier>("main"),
                                        std::move(func_block));

    program->dump(0);

    std::string c_code = program->generate_c_code(0);
    std::cout << "------------------------" << '\n';
    std::cout << c_code << '\n';

    std::string output_filename = "output.c";
    std::ofstream out_asm(output_filename);
    out_asm << c_code;
    out_asm.close();

    Tokenizer tokenizer("fn main() {}");
    while (true)
    {
        auto str = tokenizer.next_token();
        if (!str.has_value()) break;

        std::cout << str.value() << '\n';
    }

    return 0;
}
