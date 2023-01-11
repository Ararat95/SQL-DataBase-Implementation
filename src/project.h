#ifndef PROJECT_H
#define PROJECT_H

#include <list>

#include "connect.h"

class Project : public Connect {
public:
    Project(Connect*, Connect*);
    ~Project();
public:
    virtual void insert(const std::string&)override;
    virtual void update(const std::string&)override;
    virtual void del(const std::string&)override;
    virtual std::vector<std::size_t> select(const std::string&)override;
    virtual std::vector<std::size_t> get_IDs(const std::string&) override;
    virtual void print_selected(const std::vector<std::size_t>&, const std::string&) override;
private:
    void update_by_ID(const std::vector<std::string>&, const std::size_t);
    void upload_project_DB();
    void push_in_file(const std::size_t);
    void clean_file();
    void refill();
    bool check_insert_input(const std::vector<std::string>&);
    bool check_project_name(const std::string&);
    std::vector<std::size_t> search(const std::vector<std::string>&);
    void print_project_info(const std::size_t);
private:
    //object pointer to get data needed
    Connect* task;
    Connect* user;
    //flags to update DB
    bool empty_flag;
    bool update_flag;
    //parser object
    Parsing pars;
    std::map<std::string, std::size_t> project_name_ID_mapping;
    //Task data
    std::map<std::size_t, DBN::Project_info*> projects;
    //task position map
    std::map<std::size_t, std::streampos> project_pos;
    //ID counter
    static std::size_t project_ID;
    //DBfile
    std::fstream projectDB;
};

#endif //PROJECT_H