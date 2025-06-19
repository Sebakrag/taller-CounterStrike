#include "backgroundItem.h"

BackgroundItem::BackgroundItem(int width, int height, int gridSize):
        QGraphicsRectItem(0, 0, width, height), m_gridSize(gridSize) {
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // No need for scaling as we're drawing directly
    setPen(Qt::NoPen);  // No border for the main rectangle
}

QRectF BackgroundItem::boundingRect() const { return QGraphicsRectItem::boundingRect(); }

void BackgroundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                           QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Dibujar el fondo blanco
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(boundingRect());

    // Dibujar la cuadrícula
    QPen gridPen(QColor(200, 200, 200));  // Color gris claro para la cuadrícula
    gridPen.setWidth(1);
    painter->setPen(gridPen);

    // Dibujar líneas horizontales
    for (int y = 0; y <= rect().height(); y += m_gridSize) {
        painter->drawLine(rect().left(), y, rect().right(), y);
    }

    // Dibujar líneas verticales
    for (int x = 0; x <= rect().width(); x += m_gridSize) {
        painter->drawLine(x, rect().top(), x, rect().bottom());
    }
}