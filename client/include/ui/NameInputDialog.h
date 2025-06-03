#pragma once

#include <QDialog>
#include <QPixmap>

class QLabel;
class QLineEdit;
class QPushButton;

class NameInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit NameInputDialog(const QString &title,
                             const QString &labelText,
                             QWidget *parent = nullptr);
    ~NameInputDialog() override;

    bool accepted() const { return m_accepted; }

    QString textValue() const;

private slots:
    void onOkClicked();
    void onCancelClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi(const QString &labelText);
    void centerOnScreen();

    QLabel      *m_label        = nullptr;
    QLineEdit   *m_lineEdit     = nullptr;
    QPushButton *m_okButton     = nullptr;
    QPushButton *m_cancelButton = nullptr;
    bool         m_accepted     = false;
    QPixmap      m_backgroundPixmap;
};
