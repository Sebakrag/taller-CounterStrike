#ifndef MAP_SELECTION_DIALOG_H
#define MAP_SELECTION_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <vector>
#include "client/include/client_protocol.h"

class MapSelectionDialog : public QDialog {
    Q_OBJECT

public:
    MapSelectionDialog(const std::vector<ClientMapInfo>& maps, QWidget* parent = nullptr);
    
    // Retorna el nombre del archivo del mapa seleccionado
    QString selectedMapFileName() const;
    
    // Retorna el índice del mapa seleccionado en la lista original
    int selectedMapIndex() const;
    
private slots:
    void onMapSelected(QListWidgetItem* item);
    void accept() override;
    
private:
    QListWidget* mapList;
    QPushButton* selectButton;
    QPushButton* cancelButton;
    QLabel* previewLabel;
    
    std::vector<ClientMapInfo> availableMaps;
    int selectedIndex;
    QString selectedFile;
    
    void setupUI();
    void updatePreview();
};

#endif // MAP_SELECTION_DIALOG_H
