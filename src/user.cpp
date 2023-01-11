#include "user.h"
std::size_t User::ID_for_users = 0;

User::User() {
    //assigning Derived DB object to Base pointer
    update_flag = false;
    empty_flag = false;
    user_DB_upload();
    
}

void User::clean_file () {
    file_for_DB.open("../DB/user.data", std::ios::out);
    file_for_DB.clear();
    file_for_DB.close();
}

void User::refill() {
    file_for_DB.open("../DB/user.data", std::ios::app);
    if (!file_for_DB.is_open()) {
        throw std::runtime_error ("file is not found\n");
    }
        for (auto& it : users) {
            file_for_DB << '{' << '\n';
            file_for_DB << '\t' << "ID # " << it.first << '\n';
            file_for_DB << '\t' << "name # " << it.second->name << '\n';
            file_for_DB << '\t' << "surname # " << it.second->surname << '\n';
            file_for_DB << '\t' << "age # " << it.second->age << '\n';
            file_for_DB << '\t' << "email # " << it.second->email << '\n';
            file_for_DB << '\t' << "mobile # " << it.second->mobile << '\n';
            file_for_DB << '\t' << "position # " << it.second->position << '\n';
            file_for_DB << '\t' << "password # " << it.second->password << '\n';
            file_for_DB << '}' << '\n';
        }   
    file_for_DB.close();
}

User::~User() {
    //Deallocating DB pointer
     if (empty_flag) {
        clean_file();
    }
    else if (update_flag) {
        clean_file();
        refill();
    }
    for (auto it : users) {
        delete it.second;
        it.second = nullptr;
    }
}

void User::user_DB_upload() {

     // Opening user.data file
    file_for_DB.open("../DB/user.data");
    if (!file_for_DB.is_open()) {
        throw std::runtime_error("Faild to open file!");
    }
    
    std::string line = "";
    std::string tmp = "";
    while (!file_for_DB.eof()) {
        getline(file_for_DB, line);
        if ((line.find("ID") != std::string::npos)) {
           
            tmp = pars.pars_data_from_file(line);
            
            user_ID_pos.insert(std::make_pair(std::stoi(tmp), file_for_DB.tellg()));
            users[std::stoi(tmp)] = new DBN::User_info();
            ID_for_users = std::stoi(tmp);
            ++ID_for_users;
        }
        tmp = "";
        line = "";
    }
    file_for_DB.close();
    file_for_DB.open("../DB/user.data");
    std::string object_data = "";
    for (auto it : user_ID_pos) {
        // User name
        getline(file_for_DB.seekg(user_ID_pos[it.first]), object_data);
        users[it.first]->name = pars.pars_data_from_file(object_data);
        // User Surename
        getline(file_for_DB, object_data);
        users[it.first]->surname = pars.pars_data_from_file(object_data);
        // User age
        getline(file_for_DB, object_data);
        users[it.first]->age = pars.pars_data_from_file(object_data);
        // User email
        getline(file_for_DB, object_data);
        users[it.first]->email = pars.pars_data_from_file(object_data);
        email_ID.insert(std::make_pair(users[it.first]->email, it.first));
        // User phone_number
        getline(file_for_DB, object_data);
        users[it.first]->mobile = pars.pars_data_from_file(object_data);
        // User position
        getline(file_for_DB, object_data);
        users[it.first]->position = pars.pars_data_from_file(object_data);
        // User pass
        getline(file_for_DB, object_data);
        users[it.first]->password = pars.pars_data_from_file(object_data);
    }
    file_for_DB.close();
    if (file_for_DB.is_open()) {
        throw std::runtime_error("Failed to close the file");
    }
    
}

void User::push_in_file(std::size_t ID) {
    if (empty_flag) {
        update_flag = true;
    } else if (update_flag) {
        update_flag = true;
    }
    empty_flag = false;
    
    file_for_DB.open("../DB/user.data", std::ios::app);
        if (!file_for_DB.is_open()) {
            throw std::invalid_argument ("file is not found\n");
        } else {
            file_for_DB << '{' << '\n';
            user_ID_pos.insert(std::make_pair(ID, file_for_DB.tellg()));
            file_for_DB << '\t' << "ID # " << ID << '\n';
            file_for_DB << '\t' << "name # " << users[ID]->name << '\n';
            file_for_DB << '\t' << "surname # " << users[ID]->surname << '\n';
            file_for_DB << '\t' << "age # " << users[ID]->age << '\n';
            file_for_DB << '\t' << "email # " << users[ID]->email << '\n';
            file_for_DB << '\t' << "mobile # " << users[ID]->mobile << '\n';
            file_for_DB << '\t' << "position # " << users[ID]->position << '\n';
            file_for_DB << '\t' << "password # " << users[ID]->password << '\n';
            file_for_DB << '}' << '\n';
        }
        file_for_DB.close();
}

void User::insert(const std::string& src) {
    
    //making split string to pars instruction line for user 
    auto parsed = pars_user_data(src);
    //making condition to check if required fields exist
    if (check_user_input_data(parsed)) {
        //making temproary user info object to fill data 
        DBN::User_info* tmp = new DBN::User_info();
        for (auto it = parsed.begin(); it < parsed.end(); ++it) {
            if (static_cast<bool>(keys[*it])) {
                tmp->age = (keys[*it] == Keyword::AGE) ? *(it + 1) : tmp->age;
                tmp->name = (keys[*it] == Keyword::NAME) ? *(it + 1) : tmp->name;
                tmp->surname = (keys[*it] == Keyword::SURNAME) ? *(it + 1) : tmp->surname;
                tmp->position = (keys[*it] == Keyword::POSITION) ? *(it + 1) : tmp->position;
                tmp->email = (keys[*it] == Keyword::EMAIL) ? *(it + 1) : tmp->email;
                tmp->mobile = (keys[*it] == Keyword::MOBILE) ? *(it + 1) : tmp->mobile;
                tmp->password = (keys[*it] == Keyword::PASSWORD) ? *(it + 1) : tmp->password;
                ++it;
            }
        }
        users.insert(std::make_pair(ID_for_users, tmp));
        tmp = nullptr;
        email_ID.insert(std::make_pair(users[ID_for_users]->email, ID_for_users));
        push_in_file(ID_for_users);
        ++ID_for_users;
    }
    else {
        throw std::invalid_argument ("Missing neccesary arguments <Name, Surname or Email>\n");
    }
}
//checking email uniqueness
bool User::check_user_input_data(const std::vector<std::string>& data) {
    bool neccesary = false;
    bool name_check = false;
    bool surname_check = false;
    bool email_check = false;

    for (auto it = data.begin(); it < data.end(); ++it) {
        if (static_cast<bool>(keys[*it]))
        {
            if (keys[*it] == Keyword::NAME) {
                if (static_cast<bool>(keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing name\n");
                }
                name_check = true;
            }
            if (keys[*it] == Keyword::SURNAME) {
                if (static_cast<bool>(keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing surname\n");
                }
                surname_check = true;
            }
            if (keys[*it] == Keyword::EMAIL) {
                if (static_cast<bool>(keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing Email\n");
                }
                email_check = check_email(*(it + 1));
            }
        }
    }
    if (name_check && surname_check && email_check) {
        neccesary = true;
    }
    return neccesary;
}

bool User::check_email(const std::string& email) {
    for (auto& it : users) {
        if (it.second->email == email) {
            return false;
        }
    }
    return true;
}
//User update function
void User::update(const std::string& src) {
    std::vector<std::string> update_data = pars.data_for_update(src);
    std::vector<std::string> to_update = pars.data_to_update(src);
    std::vector<std::size_t> found = search(to_update);
    if (!found.empty()) {
        update_flag = true;
    }
    if (found.size() > 1) {
        for (auto& it : found) {
            print_found_data(it);
        }
        std::cout << "found users are several, do you want to update all of them? type Y/N: ";
        char ch;
        std::size_t ind;
        std::cin >> ch;
        ch = ch == 'Y' || ch == 'N' ? ch + 32 : ch;
        if (ch == 'y') {
            for (auto it = found.begin(); it < found.end(); ++it) {
                update_by_ID(update_data, *it);
            }
        }
        else if (ch == 'n') {
            std::cout << "type the ID which one you want to update: ";
            std::cin >> ind;
            bool t = false;
            for (auto& it : found) {
                if (ind == it) {
                    t = true;
                    break;
                }
            }
            if (t) {
                update_by_ID(update_data, ind);
            }
            else {
                std::cout << "wrong ID typed, try again" << std::endl;
            }
        }
    }
    else if (found.size() == 1) {
        update_by_ID(update_data, found[0]);
    }
    else {
        std::cout << "there are not users found try again" << std::endl;
    }
}
//User delete function
void User::del(const std::string& src) {
    bool all_check = false;
    bool where_check = false;
    std::string low = pars.lowercase(src);
    all_check = low.find("all") != std::string::npos ? true : false;
    where_check = low.find("where") != std::string::npos ? true : false;
    if (all_check && !where_check) {
        std::cout << "this command will remove everything in your DB, are you sure you want to continue? type Y/N to continue" << std::endl;
        char command;
        std::cin >> command;
        command = (command == 'Y' || command =='N') ? (command + 32) : command;
        switch (command) {
            case 'y': users.clear();
            update_flag = true;
            empty_flag = true;
            break;
            case 'n': std::cout << "data isn't removed please write your command correctly" << std::endl;
            break;
        }
    }
    else if (where_check) {
        auto parsed = user_delete_parser(src);
        std::vector<std::size_t> found = search(parsed);
        if (all_check) {
            std::cout << "Found users below" << std::endl;
            for (auto& it : found) {
                    print_found_data(it);
                }
            std::cout << "This command will remove all users with this data from DB, are you SURE you want to continue? Type Y/N to continue" << std::endl;
            char command;
            std::cin >> command;
            command = (command == 'Y' || command =='N') ? (command + 32) : command;
            switch (command) {
                case 'y':   delete_found_data(found);
                            update_flag = true;
                            break;
                case 'n':   std::cout << "data isn't removed please write your command correctly" << std::endl;
                            break;
            }
            if (users.empty()) {
                        empty_flag = true;
                    }
            
        }
        else {
            if (found.size() > 1) {
                std::cout << "found some users with this data, chose which you want to delete, type ID of someone" << std::endl;
                for (auto& it : found) {
                    print_found_data(it);
                }
                std::size_t id_to_delete;
                std::cin >> id_to_delete;
                if (users[id_to_delete]) {
                    std::cout << "are you sure you want to remove this user? Type Y/N to continue" << std::endl;
                    char command;
                    std::cin >> command;
                    command = (command == 'Y' || command == 'N') ? (command + 32) : command;
                    switch (command) {
                        case 'y':   users.erase(id_to_delete);
                                    update_flag = true;
                                    break;
                        case 'n':   std::cout << "You dont remove anything please try it again" << std::endl;
                                    break;
                    }
                    if (users.empty()) {
                        empty_flag = true;
                    }
                }
                else {
                    throw std::invalid_argument ("Wrong ID, this ID doesnt exist\n");
                }
            }
            else if (found.size() == 1) {
                std::cout << "found user below\n";
                print_found_data(found[0]);
                std::cout << "are you sure you want to remove this user? Type Y/N to continue" << std::endl;
                char command;
                std::cin >> command;
                command = (command == 'Y' || command == 'N') ? (command + 32) : command;
                switch (command) {
                case 'y':   users.erase(users.find(found[0]));
                            update_flag = true;
                            break;
                case 'n':   std::cout << "You dont remove anything please try it again" << std::endl;
                            break;
                }
                if (users.empty()) {
                    empty_flag = true;
                }
            }
        }
    }
    else {
        throw std::invalid_argument ("Missing keywords all or where\n");
    }
}
//User select function
std::vector<std::size_t> User::select(const std::string& src) {
    std::vector<std::size_t> found_by_data;
    if (pars.check_select(src))
    {
        for (auto& it : users) {
            // 
            found_by_data.push_back(it.first);
        }
    }
    else {
    
    auto selected_data = pars.select_parser(src);
    auto big_age = std::find_if(selected_data.begin(), selected_data.end(), [&](std::string str){return str.find('>') != std::string::npos;});
    auto small_age = std::find_if(selected_data.begin(), selected_data.end(), [&](std::string str){return str.find('<') != std::string::npos;});
    // std::cout << *big_age << std::endl;
    int big_age_data = -1;
    int small_age_data = -1;
    if (big_age != selected_data.end()) {
        small_age_data = std::stoi(pars.parser(*big_age, "> ")[1]);
    }
    if (small_age != selected_data.end()) {
        big_age_data = std::stoi(pars.parser(*small_age,"< ")[1]);
    }
    if (big_age_data != -1 || small_age_data != -1) {
        if (!pars.remove_sym_strings(selected_data).empty()) {
            selected_data = pars.remove_sym_strings(selected_data);
            found_by_data = search(selected_data);
            found_by_data = search_by_age(found_by_data, small_age_data, big_age_data);
        }
        else {
            found_by_data = search_by_age(found_by_data, small_age_data, big_age_data);
        }
    }
    else if (!selected_data.empty()) {

        found_by_data = search(selected_data);
    }
    else {
        throw std::invalid_argument ("Wrong command repeat again\n");
    }
    if (found_by_data.empty()) {
        std::cout << "Sorry there are not users found, please try again" << std::endl;
    }
    }
    return found_by_data;
}

std::vector<std::string> User::pars_user_data(const std::string& src) {
     std::string split = {"=,"};
    auto parsed = pars.user_data_parser(src, split);
    //making data identifiers and email lowercase for future changes
    for (auto it = parsed.begin(); it < parsed.end(); ++it) {
        if (static_cast<bool>(keys[pars.lowercase(*it)])) {
            *it = pars.lowercase(*it);
        }
        if (keys[*it] == Keyword::EMAIL) {
            *(it + 1) = pars.lowercase(*(it + 1));
        }
    }
    return parsed;
}

std::vector<std::string> User::user_delete_parser(const std::string& src) {
    std::string low = pars.lowercase(src);
    if (low.find("where") == std::string::npos) {
        throw std::invalid_argument("missing direction keyword\n");
    }
    int data_input_begin = low.find("where");
    ++data_input_begin;
    low = src.substr(data_input_begin, src.size());
    auto parsed = pars_user_data(low);
    return parsed;
}

std::vector<std::size_t> User::search(const std::vector<std::string>& data) {
    std::vector<std::size_t> found;
    bool check_data;
    for (auto& it : users) {
        for (auto dit = data.begin(); dit < data.end(); ++dit) {
            switch (static_cast<int>(keys[*dit])) {
                case 1: check_data = (it.second->name == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 2: check_data = (it.second->surname == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 3: check_data = (it.second->age == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 4: check_data = (it.second->email == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 5: check_data = (it.second->position == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 6: check_data = (it.second->mobile == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 7: check_data = (it.second->password == *(dit + 1)) ? true : false;
                ++dit;
                break;
                
            }
        }
        if (check_data) {
            found.push_back(it.first);
        }
    }
    return found;
}

void User::print_found_data(const std::size_t ID) {

    std::cout << "ID: " << ID << std::endl;
    std::cout << "name: " << users[ID]->name << std::endl;
    std::cout << "surname: " << users[ID]->surname << std::endl;
    std::cout << "age: " << users[ID]->age << std::endl;
    std::cout << "email: " << users[ID]->email << std::endl;
    std::cout << "position: " << users[ID]->position << std::endl;
    std::cout << "mobile: " << users[ID]->mobile << std::endl;
}

void User::print_selected(const std::vector<std::size_t>& data, const std::string& file_name) {
    for (auto& it : data) {
        print_found_data(it);
        std::cout << std::endl;
    }
}

void User::delete_found_data(const std::vector<std::size_t>& found) {
    for (auto& it : found) {
        users.erase(it);
    }
}

void User::update_by_ID(const std::vector<std::string>& update_data, const std::size_t ID) {
    for (auto dit = update_data.begin(); dit < update_data.end(); ++dit) {
        users[ID]->name = keys[*dit] == Keyword::NAME ? *(dit + 1) : users[ID]->name;
        users[ID]->surname = keys[*dit] == Keyword::SURNAME ? *(dit + 1) : users[ID]->surname;
        users[ID]->age = keys[*dit] == Keyword::AGE ? *(dit + 1) : users[ID]->age;
        users[ID]->mobile = keys[*dit] == Keyword::MOBILE ? *(dit + 1) : users[ID]->mobile;
        users[ID]->email = keys[*dit] == Keyword::EMAIL ? *(dit + 1) : users[ID]->email;
        users[ID]->password = keys[*dit] == Keyword::PASSWORD ? *(dit + 1) : users[ID]->password;
        users[ID]->position = keys[*dit] == Keyword::POSITION ? *(dit + 1) : users[ID]->position;
    }
}

std::vector<std::size_t> User::search_by_age(const std::vector<std::size_t>& found, int small, int big) {
    std::vector<std::size_t> final;
    int users_age;
    if (big == -1) {
        for (auto& it : users) {
            if (it.second->age != "EMPTY") {
                users_age = std::stoi(it.second->age);
                if (users_age > big) {
                    big = users_age;
                }
            }
        }
    }
    users_age = 0;
    if (!found.empty()) {
        for (auto& it : found) {
            if (users[it]->age != "EMPTY") {
                users_age = std::stoi(users[it]->age);
                if (users_age >= small && users_age <= big) {
                    final.push_back(it);
                }
            }
        }
        return final;
    }
    for (auto& it : users) {
        if (it.second->age != "EMPTY") {
            users_age = std::stoi(it.second->age);
            if (users_age >= small && users_age <= big) {
                final.push_back(it.first);
            }
        }
    }
    return final;
}

std::vector<std::size_t> User::get_IDs(const std::string& src) {
    auto parsed = pars.parser(src, ", ");
    std::vector<std::size_t> ids;
    for (auto& it : email_ID) {
        for (auto dit = parsed.begin(); dit < parsed.end(); ++dit) {
            if (it.first == *dit) {
                ids.push_back(it.second);
            }
        }
    }
    return ids;
}

