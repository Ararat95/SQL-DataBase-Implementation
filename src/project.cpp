#include "project.h"
std::size_t Project::project_ID = 0;

Project::Project(Connect* task, Connect* user) {
    // std::cout << "Project ctor" << std::endl;
    this->task = task;
    this->user = user;
    update_flag = false;
    empty_flag = false;
    upload_project_DB();
}

Project::~Project() {
    // std::cout << "Project dtor" << std::endl;
    if (empty_flag) {
        clean_file();
    }
    else if (update_flag) {
        clean_file();
        refill();
    }
    delete task;
}

std::vector<std::size_t> Project::get_IDs(const std::string&) {
    std::vector<std::size_t> vec;
    return vec;
}

void Project::insert(const std::string& src) {
    // std::cout << "proj insert" << std::endl;
    auto parsed = pars.task_parser(src, " ,=");
	//checking and getting file name
    auto file_name = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find('.') != std::string::npos;});
	// std::cout << *file_name << std::endl;
    std::string insert_data = src.substr(src.find('.') + 5, src.size() - 1);
    parsed = pars.task_parser(insert_data, ",={}");
    auto project_name = (std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find("project") != std::string::npos;}) + 1);
    // std::cout << *project_name << std::endl;
	*file_name = pars.lowercase(*file_name);
    // std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "##"));
    if (*file_name == "task.data") {
        if (check_project_name(*project_name)) { 
            task->insert(src);
            projects[project_name_ID_mapping[*project_name]]->tasks_in_project = task->get_IDs(*project_name);
            return;
        }
        else {
            std::cout << "sorry there are no such project, try again" << std::endl;
            return;
        }
        
    }
    else {
        auto task_data = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input == "task";});
        auto user_data = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input == "user";});
        if (task_data != parsed.end() && project_name != parsed.end() && check_project_name(*project_name)) {
            //call task spec func with proj name
            parsed.erase(task_data);
            std::string str_to_task = "insert into task.data ";
            int index_of_scope = src.find('{') + 1;
            std::string task_info = src.substr(index_of_scope, src.size() - 1);
            if (task_info.find("project") == std::string::npos) {
                str_to_task += "project = " + *project_name + ", ";
            }
            str_to_task += task_info;
            str_to_task.erase(str_to_task.find('}'));
            // std::cout << str_to_task << std::endl;
            task->insert(str_to_task);
            // std::cout << "tadam" << std::endl;
            projects[project_name_ID_mapping[*project_name]]->tasks_in_project = task->get_IDs(*project_name);
            task->print_selected(projects[project_name_ID_mapping[*project_name]]->tasks_in_project, *project_name);
            update_flag = true;
            return;
        }
        else if (task_data != parsed.end() && project_name == parsed.end()) {
            std::cout << "You try to create task, and you should mention project name" << std::endl;
            return;
        }
        else if (user_data != parsed.end() && project_name != parsed.end() && check_project_name(*project_name)) {
            std::string string_for_users = src.substr(src.find('{') + 1, src.find('}'));
            string_for_users.erase(string_for_users.find('}'));
            // std::cout << string_for_users << std::endl;
            std::vector<std::size_t> tmp = user->get_IDs(string_for_users);
            bool check = false;
            for (auto& it : tmp) {
                for (auto& nit : projects[project_name_ID_mapping[*project_name]]->users_included) {
                    if (it == nit) {
                        check = true;
                    }
                }
                  if (!check) {
                        projects[project_name_ID_mapping[*project_name]]->users_included.push_back(it);
                    }
                    check = false;
            }
            update_flag = true;
        }
        else {
            // std::cout << "got here" << std::endl;
            if (!check_insert_input(parsed)) {
                throw std::invalid_argument ("project with this name is already exist");
            }
            DBN::Project_info* tmp = new DBN::Project_info();
            for (auto it = parsed.begin(); it < parsed.end(); ++it) {
                tmp->name = (proj_keys[*it] == Project_keyword::NAME) ? *(it + 1) : tmp->name;
                tmp->deadline = (proj_keys[*it] == Project_keyword::DEADLINE) ? pars.deadline_parser(*(it + 1)) : tmp->deadline;
                tmp->description = (proj_keys[*it] == Project_keyword::DESCRIPTION) ? *(it + 1) : tmp->description;
                tmp->users_included = (proj_keys[*it] == Project_keyword::USERS) ? user->get_IDs(*(it + 1)) : tmp->users_included;

                std::time_t now = time(NULL);
                tmp->creation = std::ctime(&now);
                tmp->creation = tmp->creation.erase(tmp->creation.find('\n'));
                ++it;
            }
            // std::cout << tmp->creation;
            projects.insert(std::make_pair(project_ID, tmp));
            project_name_ID_mapping.insert(std::make_pair(tmp->name, project_ID));
            tmp = nullptr;
            push_in_file(project_ID);
            // print_task_info(project_ID);
            ++project_ID;
        }
    }
}

void Project::push_in_file(const std::size_t ID) {
    if (empty_flag) {
        update_flag = true;
    } else if (update_flag) {
        update_flag = true;
    }
    empty_flag = false;
    
    // std::cout << "project push func\n";
    projectDB.open("../DB/project.data", std::ios::app);
        if (!projectDB.is_open()) {
            throw std::invalid_argument ("file is not found\n");
        } else {
            projectDB << '{' << '\n';
            project_pos.insert(std::make_pair(ID, projectDB.tellg()));
            projectDB << '\t' << "ID # " << ID << '\n';
            projectDB << '\t' << "name # " << projects[ID]->name << '\n';
            projectDB << '\t' << "description # " << projects[ID]->description << '\n';
            projectDB << '\t' << "creation # " << projects[ID]->creation << '\n';
            projectDB << '\t' << "deadline # " << projects[ID]->deadline << '\n';
            projectDB << '\t' << "users # ";
            for (auto& it : projects[ID]->users_included) {
                projectDB << it << '|';
            }
            projectDB << '\n';
            projectDB << '\t' << "tasks # ";
            for (auto& it : projects[ID]->tasks_in_project) {
                projectDB << it << '|';
            }
            projectDB << '\n';
            projectDB << '}' << '\n';
        }
    projectDB.close();
}

bool Project::check_insert_input(const std::vector<std::string>& data) {
    bool neccesary = false;
    bool project_check = false;
    bool desc = false;

    for (auto it = data.begin(); it < data.end(); ++it) {
        if (static_cast<bool>(proj_keys[*it]))
        {
            if (proj_keys[*it] == Project_keyword::NAME) {
                //TODO
                if (static_cast<bool>(proj_keys[*(it + 1)])) {
                    throw std::invalid_argument ("Missing name\n");
                }
                project_check = !check_project_name(*(it + 1));
            }
            if (proj_keys[*it] == Project_keyword::DESCRIPTION) {
                if ((*(it + 1)).size() > 200) {
                    throw std::invalid_argument ("Description is too long, it cant be more than 150 characters long\n");
                }
            }
        }
    }
    if (project_check) {
        neccesary = true;
    }
    return neccesary;
}

bool Project::check_project_name(const std::string& name) {
    for (auto& it : projects) {
        if (it.second->name == name) {
            return true;
        }
    }
    return false;
}

 void Project::update(const std::string& src) {
    auto parsed = pars.task_parser(src, " ,=");
    //checking and getting file name
    auto file_name = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find('.') != std::string::npos;});
    if (*file_name == "task.data") {
        task->update(src);
        return;
    }
    std::vector<std::string> data_to_update = pars.updateable_data_for_task(src);
    std::vector<std::string> data_to_be_updated = pars.data_for_task_to_be_updated(src);
    std::vector<std::size_t> found_projects = search(data_to_be_updated);
    bool check = false;
    if (found_projects.empty()) {
        std::cout << "sorry there are not found projects please try again" << std::endl;
    }
    else {
        if (found_projects.size() == 1) {
            update_by_ID(data_to_update, found_projects[0]);
            update_flag = true;
        }
        else {
            for (auto& it : found_projects) {
                print_project_info(it);
            }
            std::cout << "there are several found projects choose ID to update" << std::endl;
            std::size_t id_to_update;
            std::cin >> id_to_update;
            for (auto& it : found_projects) {
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

void Project::update_by_ID(const std::vector<std::string>& update_data, const std::size_t ID) {
    for (auto dit = update_data.begin(); dit < update_data.end(); ++dit) {
        // tasks[ID]->project = task_keys[*dit] == Task_keyword::PROJECT ? *(dit + 1) : tasks[ID]->project;
        projects[ID]->name = proj_keys[*dit] == Project_keyword::NAME ? *(dit + 1) : projects[ID]->name;
        projects[ID]->description = proj_keys[*dit] == Project_keyword::DESCRIPTION ? *(dit + 1) : projects[ID]->description;
        projects[ID]->deadline = proj_keys[*dit] == Project_keyword::DEADLINE ? pars.deadline_parser(*(dit + 1)) : projects[ID]->deadline;
        // projects[ID]->users_included = proj_keys[*dit] == Project_keyword::USERS ? user->get_IDs(*(dit + 1)) : projects[ID]->users_included;
    }
}

 void Project::del(const std::string& src) {
    auto parsed = pars.task_parser(src, " ,=");
    //checking and getting file name
    auto file_name = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find('.') != std::string::npos;});
    if (*file_name == "task.data") {
        task->del(src);
        return;
    }
    std::vector<std::size_t> found_ID_of_projects;
    std::string low = pars.lowercase(src);
    bool check = false;
    if (low.find("where") == std::string::npos) {
        throw std::invalid_argument("missing keyword");
    }
     if (low.find("all") != std::string::npos && low.find("all") < low.find("where")) {
        projects.clear();
        empty_flag = true;
        return;
    }
    std::string str = src.substr((low.find("where") + 6), src.size() - 1);
    parsed = pars.task_parser(str, ",=");
    found_ID_of_projects = search(parsed);
    if (found_ID_of_projects.empty()) {
        std::cout << "there are no projects found, try again" << std::endl;
    }
    else {
        if (found_ID_of_projects.size() == 1) {
            std::cout << "are you sure you want to delete this project? press Y/N to confirm" << std::endl;
            char ch;
            std::cin >> ch;
            ch = (ch == 'Y' || ch == 'N') ? ch += 32 : ch;
            switch (ch) {
                case 'y':   projects.erase(found_ID_of_projects[0]);
                            update_flag = true;
                            break;
                case 'n':   std::cout << "nothing removed try again" << std::endl;
                            break;
            }
        }
        else {
            for (auto& it : found_ID_of_projects) {
                print_project_info(it);
            }
            std::cout << "found some tasks please type ID of projects which you want to delete" << std::endl;
            std::size_t id_to_delete_project;
            std::cin >> id_to_delete_project;
            for (auto& it : found_ID_of_projects) {
                if (id_to_delete_project == it) {
                    projects.erase(it);
                    update_flag = true;
                    check = true;
                }
            }
            if (!check) {
                std::cout << "sorry that project is not found, try again" << std::endl;
            }
        }
    }
 }
 
 std::vector<std::size_t> Project::select(const std::string& src) {

    auto parsed = pars.task_parser(src, " ,=");
	//checking and getting file name
    auto file_name = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find('.') != std::string::npos;});
    if (*file_name == "task.data") {
        return task->select(src);
    }
    
    std::vector<std::size_t> found_ID_of_projects;
    std::string low = pars.lowercase(src);
    if (low.find("where") == std::string::npos) {
        if ((low.find("all") != std::string::npos)) {
            for (auto& it : projects) {
                found_ID_of_projects.push_back(it.first);
            }
            return found_ID_of_projects;
        }
        throw std::invalid_argument("missing keyword");
    }
    std::string str = src.substr((low.find("where") + 6), src.size() - 1);
    parsed = pars.task_parser(str, ",=");
    found_ID_of_projects = search(parsed);
    for (auto& it : found_ID_of_projects) {
        std::cout << it << std::endl;
    }
    return found_ID_of_projects;
 }

 void Project::print_selected(const std::vector<std::size_t>& data, const std::string& file_name) {
    //TODO
    if (file_name == "task.data") {
        task->print_selected(data, file_name);
    }
    else {
        for (auto& it : data) {
            print_project_info(it);
        }
    }
 }

 void Project::upload_project_DB() {
     // Opening project.data file
    projectDB.open("../DB/project.data");
    if (!projectDB.is_open()) {
        throw std::runtime_error("Faild to open file!");
        // throw std::ifstream::badbit("Failed to fetch DB data");
    }
    
    std::string line = "";
    std::string tmp = "";
    while (!projectDB.eof()) {
        getline(projectDB, line);
        // std::cout << "~~" << line << "~~" << std::endl;
        if ((line.find("ID") != std::string::npos)) {
           
            tmp = pars.pars_data_from_file(line);
            
            project_pos.insert(std::make_pair(std::stoi(tmp), projectDB.tellg()));
            projects[std::stoi(tmp)] = new DBN::Project_info();
            project_ID = std::stoi(tmp);
            ++project_ID;
            // std::cout << task_ID;
        }
        tmp = "";
        line = "";
    }
    projectDB.close();
    projectDB.open("../DB/project.data");
    std::string object_data = "";
    for (auto it : project_pos) {
        // Project name
        getline(projectDB.seekg(project_pos[it.first]), object_data);
        projects[it.first]->name = pars.pars_data_from_file(object_data);
        project_name_ID_mapping.insert(std::make_pair(object_data, it.first));
        // User proj Description
        getline(projectDB, object_data);
        projects[it.first]->description = pars.pars_data_from_file(object_data);
        // creation
        getline(projectDB, object_data);
        projects[it.first]->creation = pars.pars_data_from_file(object_data);
        // deadline
        getline(projectDB, object_data);
        projects[it.first]->deadline = pars.pars_data_from_file(object_data);
        // User position
        getline(projectDB, object_data);
        projects[it.first]->users_included = pars.pars_from_file_for_ID(object_data);
        //tasks
        getline(projectDB, object_data);
        projects[it.first]->tasks_in_project = /*task->get_IDs(projects[it.first]->name);*/pars.pars_from_file_for_ID(object_data);
    }
    projectDB.close();
    // refill();
    // for (auto& it : projects) {
    //     print_project_info(it.first);
    // }
    // file_for_deleted_ID.close();
    if (projectDB.is_open()) {
        throw std::runtime_error("Failed to close the file");
    }
 }

 void Project::clean_file () {
    projectDB.open("../DB/project.data", std::ios::out);
    projectDB.clear();
    projectDB.close();
}

void Project::refill() {
    projectDB.open("../DB/project.data", std::ios::app);
        if (!projectDB.is_open()) {
            throw std::invalid_argument ("file is not found\n");
        } else {
            for (auto& it : projects) {
                projectDB << '{' << '\n';
                // project_pos.insert(std::make_pair(it.first, projectDB.tellg()));
                projectDB << '\t' << "ID # " << it.first << '\n';
                projectDB << '\t' << "name # " << it.second->name << '\n';
                projectDB << '\t' << "description # " << it.second->description << '\n';
                projectDB << '\t' << "creation # " << it.second->creation << '\n';
                projectDB << '\t' << "deadline # " << it.second->deadline << '\n';
                projectDB << '\t' << "users # ";
                for (auto& it : it.second->users_included) {
                    projectDB << it << '|';
                }
                projectDB << '\n';
                projectDB << '\t' << "tasks # ";
                for (auto& it : task->get_IDs(it.second->name)) {
                    projectDB << it << '|';
                }
                projectDB << '\n';
                projectDB << '}' << '\n';
            }
        }
    projectDB.close();
}

std::vector<std::size_t> Project::search(const std::vector<std::string>& data) {
    std::vector<std::size_t> found;
    bool check_data;
    // std::copy (data.begin(), data.end(), std::ostream_iterator<std::string>(std::cout, "$$"));
    for (auto& it : projects) {
        for (auto dit = data.begin(); dit < data.end(); ++dit) {
            switch (static_cast<int>(proj_keys[*dit])) {
                case 1: check_data = (it.second->name == *(dit + 1)) ? true : false;
                ++dit;
                break;
                case 3: check_data = (it.second->deadline == pars.deadline_parser(*(dit + 1))) ? true : false;
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

void Project::print_project_info(const std::size_t ID) {
            
    std::cout << "ID: " << ID << std::endl;
    std::cout << "name: " << projects[ID]->name << std::endl;
    std::cout << "description: " << projects[ID]->description << std::endl;
    std::cout << "creation: " << projects[ID]->creation << std::endl;
    std::cout << "deadline: " << projects[ID]->deadline << std::endl;
}
