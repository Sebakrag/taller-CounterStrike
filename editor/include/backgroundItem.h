#ifndef BACKGROUNDITEM_H
#define BACKGROUNDITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPainter>

class BackgroundItem : public QGraphicsRectItem {
public:
    BackgroundItem(int width, int height, int gridSize);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int m_gridSize;
};

#endif // BACKGROUNDITEM_H
