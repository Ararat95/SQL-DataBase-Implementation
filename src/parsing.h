#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <vector>

class Parsing {
public:
    Parsing() = default;
    ~Parsing() = default;
public:
    //task parser
    std::vector<std::string> task_parser(const std::string&, const std::string&);
    //check select all keyword
    bool check_select(const std::string&);
    //removing from vector of select data with above or low
    std::vector<std::string> remove_sym_strings(std::vector<std::string>&);
    //select parser
    std::vector<std::string> select_parser(const std::string&);
    //to_update parser
    std::vector<std::string> data_to_update(const std::string&);
    //data parser to update the old one
    std::vector<std::string> data_for_update(const std::string&);
    //data parser for user insert
    std::vector<std::string> user_data_parser(const std::string&, const std::string&);
    void trim(std::string&);
    std::string pars_data_from_file(const std::string&);
    std::string lowercase(std::string);
    std::vector<std::string> parser(const std::string&, const std::string&);
    std::string deadline_parser(const std::string&);
    std::vector<std::size_t> pars_from_file_for_ID(const std::string&);
    std::vector<std::string> updateable_data_for_task(const std::string&);
    std::vector<std::string> data_for_task_to_be_updated(const std::string&);
};

#endif //PARSING_H