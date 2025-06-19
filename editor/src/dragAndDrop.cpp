#include "dragAndDrop.h"

#include <QMessageBox>

DragAndDrop::DragAndDrop(const QPixmap& pixmap, qreal scale, QGraphicsScene* scene):
        QGraphicsPixmapItem(pixmap),
        startPos(),
        isDragging(false),
        scene(scene),
        tipoElemento(SOLID_STRUCTURE),
        teamId(0),
        weaponType(0),
        structureType(0),
        bombZoneSize(50, 50) {
    // Por defecto, los elementos no son arrastrables ni seleccionables
    // Se habilitará solo para armas en el método setTipoElemento
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setScale(scale);
}

void DragAndDrop::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // Solo permitir arrastre para elementos de tipo WEAPON
    if (event->button() == Qt::LeftButton && tipoElemento == WEAPON) {
        startPos = event->scenePos();
        originalPos = pos();  // Guardar la posición original antes del arrastre
        isDragging = true;
        setCursor(Qt::ClosedHandCursor);  // Cambiar el cursor durante el arrastre
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void DragAndDrop::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && tipoElemento == WEAPON) {
        isDragging = false;
        setCursor(Qt::OpenHandCursor);  // Restaurar el cursor de mano abierta

        // Ajustar a la cuadrícula al soltar
        if (scene) {
            QPointF currentPos = pos();
            // Redondear a la cuadrícula más cercana (asumiendo cuadrícula de 32x32)
            qreal gridX = qRound(currentPos.x() / 32.0) * 32.0;
            qreal gridY = qRound(currentPos.y() / 32.0) * 32.0;

            // Verificar si hay un tile o extra-tile en la posición donde se quiere soltar el arma
            bool tileFound = false;
            QPoint gridPos(gridX / 32, gridY / 32);

            // Buscar tiles o extra-tiles en la posición
            QList<QGraphicsItem*> itemsAtPos = scene->items(QRectF(gridX, gridY, 1, 1));
            for (QGraphicsItem* item: itemsAtPos) {
                if (item != this && item->data(1).isValid() &&
                    (item->data(1).toInt() == TILE || item->data(1).toInt() == EXTRA_TILE)) {
                    tileFound = true;
                    break;
                }
            }

            // Si no hay un tile o extra-tile, volver a la posición original
            if (!tileFound) {
                // Mostrar un mensaje de error
                QGraphicsView* view = scene->views().first();
                QMessageBox::warning(view, "Posición no válida",
                                     "Las armas solo pueden colocarse sobre tiles o extra-tiles.");

                // Volver a la posición original
                setPos(originalPos);
            } else {
                // Posición válida, actualizar posición
                setPos(gridX, gridY);
                qDebug() << "Arma movida a posición válida sobre tile:" << gridPos.x() << ","
                         << gridPos.y();
            }
        }
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void DragAndDrop::setTipoElemento(int tipo) {
    tipoElemento = tipo;  // Usa los valores de ElementType

    // Habilitar arrastre solo para elementos de tipo WEAPON
    bool isWeapon = (tipo == WEAPON);
    setFlag(QGraphicsItem::ItemIsMovable, isWeapon);
    setFlag(QGraphicsItem::ItemIsSelectable, isWeapon);
}

int DragAndDrop::getTipoElemento() const { return tipoElemento; }

// Implementación de los nuevos métodos
int DragAndDrop::getTeamId() const { return teamId; }

void DragAndDrop::setTeamId(int id) {
    teamId = id;
    // Actualizar el tipo de elemento basado en el equipo
    tipoElemento = (id == 0) ? TEAM_SPAWN_CT : TEAM_SPAWN_T;
}

int DragAndDrop::getWeaponType() const { return weaponType; }

void DragAndDrop::setWeaponType(int type) {
    weaponType = type;
    tipoElemento = WEAPON;

    // Habilitar arrastre para armas
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

int DragAndDrop::getStructureType() const { return structureType; }

void DragAndDrop::setStructureType(int type) {
    structureType = type;
    tipoElemento = SOLID_STRUCTURE;
}

QSizeF DragAndDrop::getBombZoneSize() const { return bombZoneSize; }

void DragAndDrop::setBombZoneSize(const QSizeF& size) {
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
    // Solo permitir arrastre para elementos de tipo WEAPON
    if (isDragging && tipoElemento == WEAPON) {
        QPointF newPos = mapToScene(event->pos());

        // Restringir el movimiento dentro de los límites de la escena
        setPos(constrainToScene(newPos));

        // Actualizar el rectángulo de la escena si es necesario
        scene->setSceneRect(scene->itemsBoundingRect());

        // Opcional: Mostrar una vista previa de la posición en la cuadrícula
        // durante el arrastre (podría implementarse con un rectángulo translúcido)
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