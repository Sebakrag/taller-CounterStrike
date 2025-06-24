#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QObject>
#include <QPointF>
#include "mapElements.h"

class DragAndDrop : public QGraphicsPixmapItem
{
public:
    DragAndDrop(const QPixmap &pixmap, qreal scale = 1.0, QGraphicsScene* scene = nullptr);
    int getTipoElemento() const; 
    void setTipoElemento(int tipo);
    
    // Nuevos métodos para los tipos específicos
    int getTeamId() const;
    void setTeamId(int id);
    
    int getWeaponType() const;
    void setWeaponType(int type);
    
    int getStructureType() const;
    void setStructureType(int type);
    
    QSizeF getBombZoneSize() const;
    void setBombZoneSize(const QSizeF &size);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

   // QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPointF startPos;
    QPointF originalPos; // Posición original antes de comenzar el arrastre
    bool isDragging;

    QGraphicsScene* scene;
    QMenu contextMenu;
    int tipoElemento; // Usa los valores de ElementType
    
    // Propiedades adicionales para los diferentes tipos de elementos
    int teamId;           // Para zonas de equipo (0 = CT, 1 = T)
    int weaponType;       // Para armas
    int structureType;    // Para estructuras sólidas
    QSizeF bombZoneSize;  // Para zonas de bombas

    QPointF constrainToScene(const QPointF& newPos);
};