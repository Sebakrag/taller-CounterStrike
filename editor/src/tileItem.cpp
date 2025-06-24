#include "tileItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

TileItem::TileItem(const QPixmap &pixmap, int tileId, const QString &tileName, QGraphicsScene *scene)
    : DragAndDrop(pixmap, 1.0, scene), m_tileId(tileId), m_tileName(tileName)
{
    // Configurar este elemento como un tile
    setTipoElemento(TILE_ELEMENT);
    // Permitir que sea movible
    setFlag(QGraphicsItem::ItemIsMovable, true);
    // Permitir que sea seleccionable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    // Cambiar el cursor al estar sobre el tile
    setCursor(Qt::OpenHandCursor);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Cambiar el cursor mientras se arrastra
    setCursor(Qt::ClosedHandCursor);
    // Llamar a la implementación base
    DragAndDrop::mousePressEvent(event);
}

void TileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Restaurar el cursor
    setCursor(Qt::OpenHandCursor);
    // Llamar a la implementación base
    DragAndDrop::mouseReleaseEvent(event);
}
