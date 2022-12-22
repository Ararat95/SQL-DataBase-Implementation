#include "sql.h"

int main() {

    std::string str {"insert into task.data title = title, description = dkgjbagjbagkjbapiugbs dflakjgn , a;ghfgha askgjni"};
    std::string str1 {"Insert into user.data name= Ararat, surname = Babajanyan, age = 27, email = arbasu1995@gmail.com, mobile = 096196600"};
    My_sql* ob = new My_sql();
    // ob->execute("delete all from user.data");
    std::string input;
    do {
           std::cout << "input the command: " << std::endl;
            std::getline(std::cin, input);
            if (input.empty()) {break;}
            try {
                ob->execute(input);
            }
            catch (std::invalid_argument) {
                std::cout << "something went wrong" << std::endl;
                continue;
            }
            catch (std::runtime_error) {
                std::cout << "error with files" << std::endl;
                continue;
            }
            // ob->execute(input);
            input.clear();
            std::cout << "command proccessed" << std::endl;
    } while(1);

    delete ob;
    return 0;
}