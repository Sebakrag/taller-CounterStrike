#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>

class LoginWindow: public QDialog {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow() override;

    QString userName() const;
    QString serverIp() const;
    QString serverPort() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onConnectClicked();

private:
    void centerOnScreen();

    QLineEdit* m_userEdit;
    QLineEdit* m_ipEdit;
    QLineEdit* m_portEdit;
    QPushButton* m_connectBtn;
    QLabel* m_gifLabel;
    QPixmap m_backgroundPixmap;
};
