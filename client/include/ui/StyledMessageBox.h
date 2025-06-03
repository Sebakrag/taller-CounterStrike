#pragma once

#include <QDialog>
#include <QPixmap>
#include <QString>

class QLabel;
class QPushButton;

class StyledMessageBox : public QDialog {
    Q_OBJECT

public:
    enum MessageType {
        Information,
        Warning,
        Error
    };

    explicit StyledMessageBox(const QString& title, 
                             const QString& message, 
                             MessageType type = Warning,
                             QWidget* parent = nullptr);
    ~StyledMessageBox() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUi();
    void centerOnScreen();

    QPixmap m_backgroundPixmap;
    QString m_title;
    QString m_message;
    MessageType m_type;
    
    QLabel* titleLabel;
    QLabel* messageLabel;
    QPushButton* okButton;
};

void showStyledWarning(const QString& title, const QString& message, QWidget* parent = nullptr);
