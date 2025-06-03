#pragma once

#include <QDialog>
#include <QPixmap>
#include <QListWidget>
#include <vector>
#include <string>

class Client;
class QPushButton;
class QVBoxLayout;

class JoinGameDialog : public QDialog {
    Q_OBJECT

public:
    enum JoinGameResult {
        JoinSelected = 1,
        Cancel = 0
    };

    explicit JoinGameDialog(QWidget *parent = nullptr);
    ~JoinGameDialog() override;

    QString getSelectedGame() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onJoinGame();
    void onCancel();
    void onRefreshGameList();
    void onGameSelected(QListWidgetItem* item);

private:
    void setupUi();
    void centerOnScreen();
    void updateGameList();

    QPixmap m_backgroundPixmap;
    QListWidget* gameListWidget;
    QPushButton* joinButton;
    QPushButton* cancelButton;
    QPushButton* refreshButton;
    QVBoxLayout* mainLayout;
    
    std::vector<std::string> gameList;
    QString selectedGame;
};
