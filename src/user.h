#ifndef USER_H
#define USER_H

#include "connect.h"
// #include "DB.h"

class User : public Connect {
public:
    User();
    ~User();
public:
    //User override functions
    virtual void insert(const std::string&) override;
    virtual void update(const std::string&) override;
    virtual void del(const std::string&) override;
    virtual std::vector<std::size_t> select(const std::string&) override;
    virtual void print_selected(const std::vector<std::size_t>&, const std::string&) override;
    //get users by email for task
    virtual std::vector<std::size_t> get_IDs(const std::string&) override;
private:
    //search by age 
    std::vector<std::size_t> search_by_age(const std::vector<std::size_t>&, int, int);
    // update data by ID
    void update_by_ID(const std::vector<std::string>&, const std::size_t);
    //delete found data in vector
    void delete_found_data(const std::vector<std::size_t>&);
    //print user data getting id to display info
    void print_found_data(const std::size_t); 
    //search for user by data
    std::vector<std::size_t> search(const std::vector<std::string>&);
    //parser for delete
    std::vector<std::string> user_delete_parser(const std::string&);
    //parser for user input
    std::vector<std::string> pars_user_data(const std::string&);
    //checking email
    bool check_email(const std::string&);
    bool check_user_input_data(const std::vector<std::string>&);
    
    void push_in_file(std::size_t);

    //clean file function
    virtual void clean_file();
    //refill file
    virtual void refill();
    //uploading function
    virtual void user_DB_upload();
private:
     //flags
    bool update_flag;
    bool empty_flag;
    //ID counter
    static std::size_t ID_for_users;
     //files for DB
    std::fstream file_for_DB;
private:
    std::map<std::size_t, DBN::User_info*> users;
    std::map<std::size_t, std::streampos> user_ID_pos;
    std::map<std::string, std::size_t> email_ID;
    Parsing pars;
};


#endif//USER_H