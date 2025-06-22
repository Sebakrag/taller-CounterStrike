#ifndef MAP_SELECTION_DIALOG_H
#define MAP_SELECTION_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <vector>

#include "../../../common/dtos/map_info.h"

class MapSelectionDialog : public QDialog {
    Q_OBJECT

public:
    MapSelectionDialog(QWidget* parent, const std::vector<MapInfo>& availableMaps);
    virtual ~MapSelectionDialog() = default;

    // Retorna el índice del mapa seleccionado (0-based)
    int getSelectedMapIndex() const;
    
    // Retorna el nombre del mapa seleccionado
    std::string getSelectedMapName() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUi();
    void centerOnScreen();

    QListWidget* mapList;
    QLabel* descriptionLabel;
    QPushButton* selectButton;
    QPushButton* cancelButton;
    std::vector<MapInfo> maps;
    int selectedIndex;
    QPixmap m_backgroundPixmap;
    QVBoxLayout* mainLayout;

private slots:
    void onMapSelectionChanged(int currentRow);
    void onSelectClicked();
};

#endif // MAP_SELECTION_DIALOG_H
