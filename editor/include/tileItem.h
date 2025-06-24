#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsScene>

#include "dragAndDrop.h"

// Constante para el tipo de elemento
#define TILE_ELEMENT 5  // Asumiendo que otros elementos ya tienen IDs del 0 al 4

class TileItem: public DragAndDrop {
public:
    TileItem(const QPixmap& pixmap, int tileId, const QString& tileName, QGraphicsScene* scene);

    int getTileId() const { return m_tileId; }
    QString getTileName() const { return m_tileName; }

    // Sobrescribir cualquier método necesario
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_tileId;
    QString m_tileName;
};

#endif  // TILEITEM_H
