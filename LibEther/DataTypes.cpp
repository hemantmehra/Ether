#include <LibEther/DataTypes.h>

namespace Ether {

DataTypeList* DataTypeList::m_datatype_list = nullptr;

DataTypeList::DataTypeList()
{
    int i = 0;
    m_datatype_id_map[i++] = "int"; 
    m_next_id = i;
}

DataTypeList* DataTypeList::get_instance()
{
    if (m_datatype_list == nullptr) {
        m_datatype_list = new DataTypeList();
    }
    return m_datatype_list;
}

int DataTypeList::add(std::string data_type)
{
    m_datatype_id_map[m_next_id] = data_type;
    return m_next_id++;
}

std::string DataTypeList::get(int id)
{
    if (m_datatype_id_map.find(id) != m_datatype_id_map.end()) return m_datatype_id_map[id];
    return "";
}

}
