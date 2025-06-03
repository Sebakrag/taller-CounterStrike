#pragma once

#include <QDialog>
#include <QPixmap>

namespace Ui {
class MainMenuWindow;
}

class MainMenuWindow : public QDialog {
    Q_OBJECT

public:
    enum MenuResult {
        CreateGame = 1,
        JoinGame   = 2,
        QuitApp    = 3
    };

    explicit MainMenuWindow(QWidget *parent = nullptr);
    ~MainMenuWindow() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onCreateGame();
    void onJoinGame();
    void onQuit();

private:
    void centerOnScreen();

    Ui::MainMenuWindow *ui;
    QPixmap             m_backgroundPixmap;
};
