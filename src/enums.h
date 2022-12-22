#ifndef ENUMS_H
#define ENUMS_H

//files of DB
enum class DBfile {
    USER = 1,
    PROJECT,
    TASK,
};
//commands for all
enum class Commands {
    INSERT = 1,
    UPDATE,
    SELECT,
    DELETE,
};
//User keywords
enum class Keyword {
    NAME = 1,
    SURNAME,
    AGE,
    EMAIL,
    POSITION,
    MOBILE,
    PASSWORD,
};

enum class Task_keyword {
    PROJECT = 1,
    TITLE,
    DESCRIPTION,
    DEADLINE,
    USERS,
    CREATION,
};

enum class Project_keyword {
    NAME = 1,
    DESCRIPTION,
    DEADLINE,
    CREATION,
    TASKS,
    USERS,
};

#endif //ENUMS_H