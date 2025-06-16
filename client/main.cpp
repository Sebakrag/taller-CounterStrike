#include <QApplication>
#include <exception>
#include <iostream>
#include <string>

#include "common/constants.h"
#include "include/app-state/AppStateController.h"
#include "include/client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    try {
        AppStateController controller;
        controller.update();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error en la aplicación: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
