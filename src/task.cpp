#include "task.h"
std::size_t Task::task_ID = 0;
//Task ctor
Task::Task(Connect* user) {
    // std::cout << "Task ctor" << std::endl;
    this->user = user;
    // this->DB = new TaskDB(&pars);
    task_upload();
    empty_flag = false;
    update_flag = false;
}
//Task dtor
Task::~Task() {
    // std::cout << "Task dtor" << std::endl;
    // delete DB;
    if (empty_flag) {
        clean_file();
    }
    else if (update_flag) {
        clean_file();
        refill();
    }
}

void Task::clean_file () {
    taskDB.open("../DB/task.data", std::ios::out);
    taskDB.clear();
    taskDB.close();
}

void Task::refill() {
    taskDB.open("../DB/task.data", std::ios::app);
    if (!taskDB.is_open()) {
        throw std::runtime_error ("file is not found\n");
    }
        for (auto& it : tasks) {
            taskDB << '{' << '\n';
            task_pos.insert(std::make_pair(it.first, taskDB.tellg()));
            taskDB << '\t' << "ID # " << it.first << '\n';
            taskDB << '\t' << "project # " << it.second->project << '\n';
            taskDB << '\t' << "title # " << it.second->title << '\n';
            taskDB << '\t' << "description # " << it.second->description << '\n';
            taskDB << '\t' << "creation # " << it.second->creation << std::endl;
            taskDB << '\t' << "deadline # " << it.second->deadline << '\n';
            taskDB << '\t' << "users # ";
            for (auto& it : it.second->users_included) {
                taskDB << it << '|';
            }
            taskDB << '\n';
            taskDB << '}' << '\n';
        }   
    taskDB.close();
}

void Task::insert(const std::string& src) {
    // std::cout << "task insert" << std::endl;
    auto parsed = task_insert_parser(src);
    // std::copy(std::begin(parsed), std::end(parsed), std::ostream_iterator<std::string>(std::cout, "||"));
    if (check_insert_input(parsed)) {
        DBN::Task_info* tmp = new DBN::Task_info();
        for (auto it = parsed.begin(); it < parsed.end(); ++it) {
            tmp->project = (task_keys[*it] == Task_keyword::PROJECT) ? *(it + 1) : tmp->project;
            tmp->title = (task_keys[*it] == Task_keyword::TITLE) ? *(it + 1) : tmp->title;
            tmp->deadline = (task_keys[*it] == Task_keyword::DEADLINE) ? pars.deadline_parser(*(it + 1)) : tmp->deadline;
            tmp->description = (task_keys[*it] == Task_keyword::DESCRIPTION) ? *(it + 1) : tmp->description;
            tmp->users_included = (task_keys[*it] == Task_keyword::USERS) ? user->get_IDs(*(it + 1)) : tmp->users_included;
            std::time_t now = time(NULL);
            tmp->creation = std::ctime(&now);
            tmp->creation = tmp->creation.erase(tmp->creation.find('\n'));
            ++it;
        }
        // std::cout << tmp->creation;
        tasks.insert(std::make_pair(task_ID, tmp));
        tmp = nullptr;
        push_in_file(task_ID);
        print_task_info(task_ID);
        ++task_ID;
    }
}

std::string Task::date_to_str(const struct tm* data) {
    std::string date;
    date = std::to_string(data->tm_mday) + '.' +std::to_string(data->tm_mon + 1) + '.' + std::to_string(data->tm_year + 1900);
    return date;
}

void Task::update(const std::string& src) {
    std::vector<std::string> data_to_update = pars.updateable_data_for_task(src);
    std::vector<std::string> data_to_be_updated = pars.data_for_task_to_be_updated(src);
    std::vector<std::size_t> found_tasks = search(data_to_be_updated);
    bool check = false;
    if (found_tasks.empty()) {
        std::cout << "sorry there are not found tasks please try again" << std::endl;
    }
    else {
        if (found_tasks.size() == 1) {
            update_by_ID(data_to_update, found_tasks[0]);
            update_flag = true;
        }
        else {
            for (auto& it : found_tasks) {
                print_task_info(it);
            }
            std::cout << "there are several found tasks choose ID to update" << std::endl;
            std::size_t id_to_update;
            std::cin >> id_to_update;
            for (auto& it : found_tasks) {
                if (it == id_to_update) {
                    update_by_ID(data_to_update, it);
                    update_flag = true;
                    check = true;
                }
            }
            if (!check) {
                std::cout << "sorry wrong id, try again" << std::endl;
            }
        }
    }
}

void Task::del(const std::string& src) {
    std::vector<std::size_t> found_ID_of_tasks;
    std::string low = pars.lowercase(src);
    bool check = false;
    if (low.find("where") == std::string::npos) {
        throw std::invalid_argument("missing keyword");
    }
     if (low.find("all") != std::string::npos && low.find("all") < low.find("where")) {
        tasks.clear();
        empty_flag = true;
        return;
    }
    std::string str = src.substr((low.find("where") + 6), src.size() - 1);
    auto parsed = pars.task_parser(str, ",=");
    found_ID_of_tasks = search(parsed);
    if (found_ID_of_tasks.empty()) {
        std::cout << "there are no tasks found, try again" << std::endl;
    }
    else {
        if (found_ID_of_tasks.size() == 1) {
            std::cout << "are you sure you want to delete this task? press Y/N to confirm" << std::endl;
            char ch;
            std::cin >> ch;
            ch = (ch == 'Y' || ch == 'N') ? ch += 32 : ch;
            switch (ch) {
                case 'y':   tasks.erase(found_ID_of_tasks[0]);
                            update_flag = true;
                            break;
                case 'n':   std::cout << "nothing removed try again" << std::endl;
                            break;
            }
        }
        else {
            for (auto& it : found_ID_of_tasks) {
                print_task_info(it);
            }
            std::cout << "found some tasks please type ID of task which you want to delete" << std::endl;
            std::size_t id_to_delete_task;
            std::cin >> id_to_delete_task;
            for (auto& it : found_ID_of_tasks) {
                if (id_to_delete_task == it) {
                    tasks.erase(it);
                    update_flag = true;
                    check = true;
                }
            }
            if (!check) {
                std::cout << "sorry that task is not found, try again" << std::endl;
            }
        }
    }
}

std::vector<std::size_t> Task::select(const std::string& src) { 
    std::vector<std::size_t> found_ID_of_tasks;
    std::string low = pars.lowercase(src);
    if (low.find("where") == std::string::npos) {
        if ((low.find("all") != std::string::npos)) {
            for (auto& it : tasks) {
                found_ID_of_tasks.push_back(it.first);
            }
            return found_ID_of_tasks;
        }
        throw std::invalid_argument("missing keyword");
    }
    std::string str = src.substr((low.find("where") + 6), src.size() - 1);
    auto parsed = pars.task_parser(str, ",=");
    found_ID_of_tasks = search(parsed);
    for (auto& it : found_ID_of_tasks) {
        std::cout << it << std::endl;
    }
    return found_ID_of_tasks;
}

void Task::print_selected(const std::vector<std::size_t>& data, const std::string& file_name) {
    for (auto& it : data) {
        print_task_info(it);
    }
}

std::vector<std::size_t> Task::get_IDs(const std::string& src) {
    std::vector<std::size_t> found;
    for (auto& it : tasks) {
        if (it.second->project == src) {
            found.push_back(it.first);
        }
    }
    return found;
}

std::vector<std::string> Task::task_insert_parser(const std::string& src) {
    std::string low = pars.lowercase(src);
    std::string str = src.substr((low.find('.') + 5), low.size() - 1);
    
    auto parsed = pars.task_parser(str, ",=");
    // std::copy(std::begin(parsed), std::end(parsed), std::ostream_iterator<std::string>(std::cout, "||"));
    
    // std::copy(std::begin(parsed), std::end(parsed), std::ostream_iterator<std::string>(std::cout, "||"));
    return parsed;
}

bool Task::check_insert_input(const std::vector<std::string>& data) {
    bool neccesary = false;
    bool project_check = false;
    bool title_check = false;
    bool desc = false;

    for (auto it = data.begin(); it < data.end(); ++it) {
        if (static_cast<bool>(task_keys[*it]))
        {
            if (task_keys[*it] == Task_keyword::PROJECT) {
                //TODO
                if (static_cast<bool>(task_keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing project\n");
                }
                project_check = true;
            }
            if (task_keys[*it] == Task_keyword::TITLE) {
                if (static_cast<bool>(task_keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing title\n");
                }
                title_check = check_title(*(it + 1));
            }
            if (task_keys[*it] == Task_keyword::DESCRIPTION) {
                if ((*(it + 1)).size() > 200) {
                    throw std::invalid_argument ("Description is too long, it cant be more than 150 characters long\n");
                }
            }

        }
    }
    if (project_check && title_check) {
        neccesary = true;
    }
    return neccesary;
}

void Task::print_task_info(const std::size_t ID) {
            
    std::cout << "ID: " << ID << std::endl;
    std::cout << "project: " << tasks[ID]->project << std::endl;
    std::cout << "title: " << tasks[ID]->title << std::endl;
    std::cout << "description: " << tasks[ID]->description << std::endl;
    std::cout << "creation: " << tasks[ID]->creation << std::endl;
    std::cout << "deadline: " << tasks[ID]->deadline << std::endl;
    //user->print_selected(tasks[ID]->users_included);
}

void Task::push_in_file(std::size_t ID) {
    
    if (empty_flag) {
        update_flag = true;
    } else if (update_flag) {
        update_flag = true;
    }
    empty_flag = false;
    
    std::cout << "task push func\n";
    taskDB.open("../DB/task.data", std::ios::app);
        if (!taskDB.is_open()) {
            throw std::invalid_argument ("file is not found\n");
        } else {
            taskDB << '{' << '\n';
            task_pos.insert(std::make_pair(ID, taskDB.tellg()));
            taskDB << '\t' << "ID # " << ID << '\n';
            taskDB << '\t' << "project # " << tasks[ID]->project << '\n';
            taskDB << '\t' << "title # " << tasks[ID]->title << '\n';
            taskDB << '\t' << "description # " << tasks[ID]->description << '\n';
            taskDB << '\t' << "creation # " << tasks[ID]->creation << '\n';
            taskDB << '\t' << "deadline # " << tasks[ID]->deadline << '\n';
            taskDB << '\t' << "users # ";
            for (auto& it : tasks[ID]->users_included) {
                taskDB << it << '|';
            }
            taskDB << '\n';
            taskDB << '}' << '\n';
        }
    taskDB.close();
}

void Task::task_upload() {
    // Opening user.data file
    taskDB.open("../DB/task.data");
    if (!taskDB.is_open()) {
        throw std::runtime_error("Faild to open file!");
        // throw std::ifstream::badbit("Failed to fetch DB data");
    }
    
    std::string line = "";
    std::string tmp = "";
    while (!taskDB.eof()) {
        getline(taskDB, line);
        // std::cout << "~~" << line << "~~" << std::endl;
        if ((line.find("ID") != std::string::npos)) {
           
            tmp = pars.pars_data_from_file(line);
            
            task_pos.insert(std::make_pair(std::stoi(tmp), taskDB.tellg()));
            tasks[std::stoi(tmp)] = new DBN::Task_info();
            task_ID = std::stoi(tmp);
            ++task_ID;
            // std::cout << task_ID;
        }
        tmp = "";
        line = "";
    }
    taskDB.close();
    taskDB.open("../DB/task.data");
    std::string object_data = "";
    for (auto it : task_pos) {
        // User name
        getline(taskDB.seekg(task_pos[it.first]), object_data);
        tasks[it.first]->project = pars.pars_data_from_file(object_data);
        // User Surename
        getline(taskDB, object_data);
        tasks[it.first]->title = pars.pars_data_from_file(object_data);
        // User age
        getline(taskDB, object_data);
        tasks[it.first]->description = pars.pars_data_from_file(object_data);
        // User email
        getline(taskDB, object_data);
        tasks[it.first]->creation = pars.pars_data_from_file(object_data);
        // User phone_number
        getline(taskDB, object_data);
        tasks[it.first]->deadline = pars.pars_data_from_file(object_data);
        // User position
        getline(taskDB, object_data);
        tasks[it.first]->users_included = pars.pars_from_file_for_ID(object_data);
    }
    taskDB.close();
    // file_for_deleted_ID.close();
    if (taskDB.is_open()) {
        throw std::runtime_error("Failed to close the file");
    }
}

std::vector<std::size_t> Task::search(const std::vector<std::string>& data) {
    std::vector<std::size_t> found;
    bool check_data;
    // std::copy (data.begin(), data.end(), std::ostream_iterator<std::string>(std::cout, "$$"));
    for (auto& it : tasks) {
        for (auto dit = data.begin(); dit < data.end(); ++dit) {
            switch (static_cast<int>(task_keys[*dit])) {
                case 1: check_data = (it.second->project == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 2: check_data = (it.second->title == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 4: check_data = (it.second->deadline == pars.deadline_parser(*(dit + 1))) ? true : false;
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

void Task::update_by_ID(const std::vector<std::string>& update_data, const std::size_t ID) {
    for (auto dit = update_data.begin(); dit < update_data.end(); ++dit) {
        // tasks[ID]->project = task_keys[*dit] == Task_keyword::PROJECT ? *(dit + 1) : tasks[ID]->project;
        tasks[ID]->title = task_keys[*dit] == Task_keyword::TITLE ? *(dit + 1) : tasks[ID]->title;
        tasks[ID]->description = task_keys[*dit] == Task_keyword::DESCRIPTION ? *(dit + 1) : tasks[ID]->description;
        tasks[ID]->deadline = task_keys[*dit] == Task_keyword::DEADLINE ? pars.deadline_parser(*(dit + 1)) : tasks[ID]->deadline;
        tasks[ID]->users_included = task_keys[*dit] == Task_keyword::USERS ? user->get_IDs(*(dit + 1)) : tasks[ID]->users_included;
    }
}

bool Task::check_title(const std::string& title) {
    for (auto& it : tasks) {
        if (it.second->title == title) {
            throw std::invalid_argument ("please choose another title");
        }
    }
    return true;
}
