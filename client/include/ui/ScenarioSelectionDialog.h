#ifndef SCENARIO_SELECTION_DIALOG_H
#define SCENARIO_SELECTION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>
#include <vector>

class ScenarioSelectionDialog: public QDialog {
    Q_OBJECT

public:
    ScenarioSelectionDialog(QWidget* parent, const std::vector<std::string>& availableScenarios);
    virtual ~ScenarioSelectionDialog() = default;

    // Retorna el nombre del escenario seleccionado
    std::string getSelectedScenario() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUi();
    void centerOnScreen();

    QListWidget* scenarioList;
    QPushButton* selectButton;
    QPushButton* cancelButton;
    std::vector<std::string> scenarios;
    int selectedIndex;
    QPixmap m_backgroundPixmap;
    QVBoxLayout* mainLayout;

private slots:
    void onScenarioSelectionChanged(int currentRow);
    void onSelectClicked();
};

#endif  // SCENARIO_SELECTION_DIALOG_H
