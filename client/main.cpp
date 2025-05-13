#include <exception>
#include <iostream>

#include "common/constants.h"

#include "AppStateController.h"

int main() {
    try {
        AppStateController app_sc;
        app_sc.update();

        std::cout << "AppStateController finish his job" << std::endl;
        return SUCCESS;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ERROR;
    }
}
