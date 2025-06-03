#include <exception>
#include <iostream>
#include <string>

#include "include/client.h"

#include "include/app-state/AppStateController.h"
#include <QApplication>
#include "common/constants.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        AppStateController controller;
        controller.update();
        return app.exec();
    } catch (const std::exception &e) {
        std::cerr << "Error en la aplicación: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}