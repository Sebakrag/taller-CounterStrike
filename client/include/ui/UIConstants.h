#pragma once

#include <QSize>
#include <QString>

namespace UIConstants {
// Tamaños para ventanas redimensionables
const QSize DEFAULT_SIZE(800, 600);
const QSize MINIMUM_SIZE(640, 480);

// Estilos comunes
const QString BUTTON_STYLE = R"(
        QPushButton {
            background-color: #A9A9A9;
            color: white;
            border-radius: 5px;
            padding: 5px 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #8B8B8B;
        }
        QPushButton:pressed {
            background-color: #696969;
        }
    )";

const QString LIST_STYLE = R"(
        QListWidget {
            background-color: rgba(0, 0, 0, 150);
            color: white;
            border-radius: 5px;
            padding: 5px;
        }
        QListWidget::item {
            height: 25px;
        }
        QListWidget::item:selected {
            background-color: rgba(100, 100, 100, 150);
        }
    )";

const QString LABEL_STYLE = "color: white; font-size: 14px;";
const QString TITLE_STYLE = "color: white; font-size: 18px; font-weight: bold;";
}  // namespace UIConstants
