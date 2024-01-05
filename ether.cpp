#include <fstream>
#include <iostream>
#include <LibEther/AST.h>
#include <LibEther/DataTypes.h>
#include <LibEther/Parser.h>
#include <LibEther/Tokenizer.h>

#define MAIN_DEBUG

template <typename T, typename... Args>
std::unique_ptr<T> make(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main(int argc, char* argv[])
{
    using namespace Ether;
    if (argc < 2) {
        std::cout << "Usage: bolt program.bolt" << '\n';
        return 1;
    }

    std::string filename(argv[1]);
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        std::cout << "File: \'" << filename << "\' does not exist" << '\n';
        return 1;
    }
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

#ifdef MAIN_DEBUG
    std::cout << "[Read] " << filename << "\nFile length " << content.length() << '\n';
#endif

    ifs.close();

    auto datatype_list = DataTypeList::get_instance();
    // datatype_list->add("struct Vector");

    // auto program = make<Program>();

    // auto struct_block = make<BlockStatement>();
    // auto func_block = make<BlockStatement>();

    // struct_block->append<VariableDeclaration>(
    //     0, std::make_shared<Identifier>("x"), nullptr
    // );
    // struct_block->append<VariableDeclaration>(
    //     0, std::make_shared<Identifier>("y"), nullptr
    // );

    // func_block->append<VariableDeclaration>(
    //     0, std::make_shared<Identifier>("a"), std::make_shared<Literal>(42)
    // );
    // func_block->append<VariableDeclaration>(
    //     1, std::make_shared<Identifier>("v1"), nullptr
    // );
    // func_block->append<ReturnStatement>(std::make_shared<Identifier>("a"));

    // program->append<StructDeclaration>(std::make_shared<Identifier>("Vector"),
    //                                     std::move(struct_block));
    // program->append<FunctionDeclaration>(0,
    //                                     std::make_shared<Identifier>("main"),
    //                                     std::move(func_block));
    // program->dump(0);

    // std::string c_code = program->generate_c_code(0);
    // std::cout << "------------------------" << '\n';
    // std::cout << c_code << '\n';

    std::string output_filename = "output.c";

    Tokenizer tokenizer(content);
    std::vector<Token> tokens;
    while (true)
    {
        auto token = tokenizer.next_token();
        if (!token.has_value()) break;
        tokens.push_back(token.value());

        std::cout << token.value().to_string() << '\n';
    }

    std::cout << "------------------------" << '\n';

    Parser parser(tokens);
    auto program = parser.parse();
    auto errors = parser.get_errors();

    if (errors.size() > 0) {
        for(auto err: errors) {
            std::cout << err.to_string() << '\n';
        }
        return 1;
    }

    program->dump(0);

    std::string c_code = program->generate_c_code(0);
    std::cout << "------------------------" << '\n';
    std::cout << c_code << '\n';

    std::ofstream out_asm(output_filename);
    out_asm << c_code;
    out_asm.close();

    return 0;
}
