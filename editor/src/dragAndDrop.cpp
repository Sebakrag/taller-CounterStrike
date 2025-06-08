#include "dragAndDrop.h"

DragAndDrop::DragAndDrop(const QPixmap &pixmap, qreal scale, QGraphicsScene* scene)
    : QGraphicsPixmapItem(pixmap), scene(scene), tipoElemento(SOLID_STRUCTURE),
      teamId(0), weaponType(0), structureType(0), bombZoneSize(50, 50)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setScale(scale);
}

void DragAndDrop::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        startPos = event->scenePos();
        isDragging = true;
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void DragAndDrop::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void DragAndDrop::setTipoElemento(int tipo) {
    tipoElemento = tipo; // Usa los valores de ElementType
}

int DragAndDrop::getTipoElemento() const {
    return tipoElemento;
}

// Implementación de los nuevos métodos
int DragAndDrop::getTeamId() const {
    return teamId;
}

void DragAndDrop::setTeamId(int id) {
    teamId = id;
    // Actualizar el tipo de elemento basado en el equipo
    tipoElemento = (id == 0) ? TEAM_SPAWN_CT : TEAM_SPAWN_T;
}

int DragAndDrop::getWeaponType() const {
    return weaponType;
}

void DragAndDrop::setWeaponType(int type) {
    weaponType = type;
    tipoElemento = WEAPON;
}

int DragAndDrop::getStructureType() const {
    return structureType;
}

void DragAndDrop::setStructureType(int type) {
    structureType = type;
    tipoElemento = SOLID_STRUCTURE;
}

QSizeF DragAndDrop::getBombZoneSize() const {
    return bombZoneSize;
}

void DragAndDrop::setBombZoneSize(const QSizeF &size) {
    bombZoneSize = size;
    tipoElemento = BOMB_ZONE;
}


/*
QVariant DragAndDrop::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange && scene()) {
        QRectF newRect = boundingRect();
        newRect.setWidth(newRect.width() * 1.5); 
        newRect.setHeight(newRect.height() * 1.5);
        prepareGeometryChange();
        setPos(mapToScene(newRect.center()));
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}

*/

void DragAndDrop::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging) {
        QPointF newPos = mapToScene(event->pos());

        setPos(constrainToScene(newPos));

        scene->setSceneRect(scene->itemsBoundingRect());
    }
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

QPointF DragAndDrop::constrainToScene(const QPointF& newPos) {
    QRectF viewRect = scene->views().first()->viewport()->rect();
    QRectF itemRect = boundingRect();

    qreal x = qBound(viewRect.left(), newPos.x(), viewRect.right() - itemRect.width());
    qreal y = qBound(viewRect.top(), newPos.y(), viewRect.bottom() - itemRect.height());

    return QPointF(x, y);
}