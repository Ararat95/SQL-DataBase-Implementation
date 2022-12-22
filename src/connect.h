#ifndef CONNECT_H
#define CONNECT_H


#include "DB.h"


class Connect {

public:
    Connect();
    virtual ~Connect();
public:
    virtual void insert(const std::string&) = 0;
    virtual void update(const std::string&) = 0;
    virtual void del(const std::string&) = 0;
    virtual std::vector<std::size_t> select(const std::string&) = 0;
    virtual std::vector<std::size_t> get_IDs(const std::string&) = 0;
    virtual void print_selected(const std::vector<std::size_t>&, const std::string&) = 0;
private:
    void init();
protected:
    std::map<std::string, Keyword> keys;
    std::map<std::string,Task_keyword> task_keys;
    std::map<std::string, Project_keyword> proj_keys;
};

#endif //CONNECT_H