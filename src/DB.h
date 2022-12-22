#ifndef DB_H
#define DB_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <ctime>

#include "enums.h"
#include "parsing.h"

namespace DBN {
    struct User_info {
        std::string name = "EMPTY";
        std::string surname = "EMPTY";
        std::string email = "EMPTY";
        std::string mobile = "EMPTY";
        std::string position = "EMPTY";
        std::string password = "EMPTY";
        std::string age = "EMPTY";
    };
    struct Task_info {
        std::string project{"EMPTY"};
        std::string title{"EMPTY"};
        std::string description{"EMPTY"};
        std::string creation{"EMPTY"};
        std::string deadline{"EMPTY"};
        std::vector<std::size_t> users_included;
    };
    struct Project_info {
        std::string name{"EMPTY"};
        std::string description{"EMPTY"};
        std::string creation{"EMPTY"};
        std::string deadline{"EMPTY"};
        std::vector<std::size_t> users_included;
        std::vector<std::size_t> tasks_in_project;
    };
};

#endif // DB_H