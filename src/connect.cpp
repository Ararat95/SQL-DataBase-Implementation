#include "connect.h"

Connect::Connect() {
    init(); 
}

Connect::~Connect() {
    
}

void Connect::init() {
    //initialize Keywords
    keys["name"] = Keyword::NAME;
    keys["surname"] = Keyword::SURNAME;
    keys["age"] = Keyword::AGE;
    keys["email"] = Keyword::EMAIL;
    keys["position"] = Keyword::POSITION;
    keys["password"] = Keyword::PASSWORD;
    keys["mobile"] = Keyword::MOBILE;

    //Initialize task keywords
    task_keys["project"] = Task_keyword::PROJECT;
    task_keys["title"] = Task_keyword::TITLE;
    task_keys["description"] = Task_keyword::DESCRIPTION;
    task_keys["deadline"] = Task_keyword::DEADLINE;
    task_keys["users"] = Task_keyword::USERS;
    task_keys["creation"] = Task_keyword::CREATION;

    //initialie project keywords
    proj_keys["name"] = Project_keyword::NAME;
    proj_keys["description"] = Project_keyword::DESCRIPTION;
    proj_keys["creation"] = Project_keyword::CREATION;
    proj_keys["deadline"] = Project_keyword::DEADLINE;
    proj_keys["tasks"] = Project_keyword::TASKS;
    proj_keys["users"] = Project_keyword::USERS;
}

