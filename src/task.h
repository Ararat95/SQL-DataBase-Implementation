#ifndef TASK_H
#define TASK_H

#include "connect.h"

class Task : public Connect {
public:
    Task(Connect*);
    ~Task();
public:
    virtual void insert(const std::string&)override;
    virtual void update(const std::string&)override;
    virtual void del(const std::string&)override;
    virtual std::vector<std::size_t> select(const std::string&)override;
    virtual std::vector<std::size_t> get_IDs(const std::string&) override;
    virtual void print_selected(const std::vector<std::size_t>&, const std::string&) override;
private:
    void update_by_ID(const std::vector<std::string>&, const std::size_t);
    void clean_file();
    void refill();
    std::vector<std::size_t> search(const std::vector<std::string>& data);
    void print_task_info(const std::size_t);
    std::string date_to_str(const struct tm*);
    void push_in_file(std::size_t);
    //insert parser
    std::vector<std::string> task_insert_parser(const std::string&);
    //task input checking
    bool check_insert_input(const std::vector<std::string>&);
    void task_upload();
    bool check_title(const std::string&);
private:
    bool empty_flag;
    bool update_flag;
    Parsing pars;
    Connect* user;
    //Task data
    std::map<std::size_t, DBN::Task_info*> tasks;
    //task position map
    std::map<std::size_t, std::streampos> task_pos;
    static std::size_t task_ID;
    std::fstream taskDB;
};

#endif//TASK_H