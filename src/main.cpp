#include "sql.h"

int main() {
    My_sql* ob = new My_sql();
    std::string input;
    do {
           std::cout << "input the command: " << std::endl;
            std::getline(std::cin, input);
            if (input.empty()) {
                break;
            }
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
            input.clear();
            std::cout << "command proccessed" << std::endl;
    } while(1);

    delete ob;
    return 0;
}