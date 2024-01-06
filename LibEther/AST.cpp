#include <iostream>
#include <sstream>
#include <LibEther/AST.h>

namespace Ether {

std::string get_indent_string(int indent)
{   
    std::stringstream ss;
    for(int i=0; i<indent * 2; i++)
        ss << ' ';
    return ss.str();
}

std::string ScopeNode::generate_c_code(int indent) const
{
    std::stringstream ss;
    auto children = m_children;
    for(auto&& i: children) {
        ss << i->generate_c_code(indent);
    }
    return ss.str();
}

std::string StructDeclaration::generate_c_code(int indent) const
{
    std::stringstream ss;
    std::string indent_str = get_indent_string(indent);

    ss << indent_str << "struct " << name() << '\n';
    ss << indent_str << "{\n";
    ss << body().generate_c_code(indent + 1);
    ss << indent_str << "};\n\n";

    return ss.str();
}

std::string FunctionDeclaration::generate_c_code(int indent) const
{
    auto datatype_list = DataTypeList::get_instance();
    std::stringstream ss;
    std::string indent_str = get_indent_string(indent);

    ss << indent_str << datatype_list->get(m_datatype_id) << " " << name() << "()" << '\n';
    ss << indent_str << "{\n";
    ss << body()->generate_c_code(indent + 1);
    ss << indent_str << "}\n";

    return ss.str();
}

std::string VariableDeclaration::generate_c_code(int indent) const
{
    auto datatype_list = DataTypeList::get_instance();
    std::stringstream ss;
    std::string indent_str = get_indent_string(indent);

    ss << indent_str << datatype_list->get(m_datatype_id) << " " << identifier().generate_c_code(indent);
    if (expression() != nullptr) {
        ss << " = " << expression()->generate_c_code(indent);
    }
    ss << ";\n";
    return ss.str();
}

std::string StructMemberDeclaration::generate_c_code(int indent) const
{
    auto datatype_list = DataTypeList::get_instance();
    std::stringstream ss;
    std::string indent_str = get_indent_string(indent);

    ss << indent_str << datatype_list->get(m_datatype_id) << " " << identifier().generate_c_code(indent);
    ss << ";\n";
    return ss.str();
}

std::string ReturnStatement::generate_c_code(int indent) const
{
    std::stringstream ss;
    ss << get_indent_string(indent);
    ss << "return " << argument().generate_c_code(indent) << ";\n";
    return ss.str();
}

std::string Identifier::generate_c_code(int indent) const
{
    (void) indent;
    std::stringstream ss;
    ss << m_name;
    return ss.str();
}

std::string Literal::generate_c_code(int indent) const
{
    (void) indent;
    std::stringstream ss;
    ss << m_value;
    return ss.str();
}

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

void StructDeclaration::dump(int indent) const
{
    print_indent(indent);
    std::cout << "StructDeclaration: " << name() << '\n';
    body().dump(indent + 1);
}

void FunctionDeclaration::dump(int indent) const
{
    print_indent(indent);
    std::cout << "FunctionDeclaration: " << name() << '\n';
    body()->dump(indent + 1);
}

void VariableDeclaration::dump(int indent) const
{
    print_indent(indent);
    std::cout << "VaribleDeclaration: " << "\n";
    identifier().dump(indent + 1);
    // expression().dump(indent + 1);
}

void StructMemberDeclaration::dump(int indent) const
{
    print_indent(indent);
    std::cout << "StructMemberDeclaration: " << "\n";
    identifier().dump(indent + 1);
}

void ReturnStatement::dump(int indent) const
{
    print_indent(indent);
    std::cout << "ReturnStatement" << '\n';
    argument().dump(indent+1);
}

void Identifier::dump(int indent) const
{
    print_indent(indent);
    std::cout << m_name << "\n";
}

void Literal::dump(int indent) const
{
    print_indent(indent);
    std::cout << "Literal: " << m_value << '\n';
}

}
