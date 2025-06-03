#pragma once

#include <QDialog>
#include <QPixmap>
#include <QTimer>
#include <vector>

#include "../../common/types.h"

class Client;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QListWidget;

class LobbyWindow : public QDialog {
    Q_OBJECT

public:
    enum LobbyResult {
        StartGame = 1,
        LeaveGame = 2
    };

    explicit LobbyWindow(Client* client, QWidget *parent = nullptr);
    ~LobbyWindow() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onStartGame();
    void onLeaveGame();
    void onRefreshPlayers();

private:
    void setupUi();
    void centerOnScreen();
    void updatePlayerLists();

    Client* client;
    QTimer refreshTimer;
    QPixmap m_backgroundPixmap;
    
    // UI elements
    QLabel* titleLabel;
    QPushButton* startButton;
    QPushButton* leaveButton;
    QPushButton* refreshButton;
    QListWidget* terroristList;
    QListWidget* counterTerroristList;
    QLabel* terroristLabel;
    QLabel* counterTerroristLabel;
    
    // Layouts
    QVBoxLayout* mainLayout;
    
    // Player data
    std::vector<PlayerInfoLobby> players;
    bool isCreator;
};
