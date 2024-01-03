#pragma once

#include <map>
#include <string>
#include <memory>

namespace Ether {

class DataTypeList
{
public:
    static DataTypeList* get_instance();

    // void operator=(const DataTypeList &) = delete;
    // DataTypeList(DataTypeList &other) = delete;

    int add(std::string);
    std::string get(int);

private:
    static DataTypeList* m_datatype_list;
    int m_next_id;
    std::map<int, std::string> m_datatype_id_map;
    DataTypeList();
};

}
