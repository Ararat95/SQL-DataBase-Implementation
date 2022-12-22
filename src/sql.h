#ifndef SQL_H
#define SQL_H

/*
* @author Ararat Babajanyan
* @file Git_works/ Wolverine_tasks/ My_SQL/ src/
* this file contains implementation of Unique SQL DB system
* license ......
*/

// #include <algorithm>

#include "user.h"
#include "task.h"
#include "project.h"

class My_sql {
public:
    My_sql();
    ~My_sql();
public:
    using func_ptr = void (My_sql::*)(Connect*, const std::string&); 
    void execute(const std::string&);
private:
    // void call_obj(Connect*, const std::vector<std::string>&, const std::string&);
    void check_file_name(const std::string&);
    void init();
    void insert (Connect*, const std::string&);
    void update(Connect*, const std::string&);
    void del(Connect*, const std::string&);
    void select(Connect*, const std::string&);
private:
    Parsing ob;
    std::map<Commands, func_ptr> f_ptr;
    std::map<std::string, Commands> func;
    std::map<std::string, DBfile> DB_map;
    std::map<DBfile, Connect*> objects;
};


#endif//SQL_H
