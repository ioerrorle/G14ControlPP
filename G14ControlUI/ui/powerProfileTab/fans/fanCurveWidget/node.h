#ifndef G14CONTROLPP_NODE_H
#define G14CONTROLPP_NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class Edge;
class FanCurveView;

class Node : public QGraphicsItem {
public:
    Node(FanCurveView *fanCurveView);

    void setLeftEdge(Edge *left);
    void setRightEdge(Edge *right);
    Edge *getLeftEdge() const;
    Edge *getRightEdge() const;

    enum {
        Type = UserType + 1
    };

    int type() const override { return Type; }

//    void calculateForces();

    bool advancePosition();

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

    QPointF newPos;
    FanCurveView *graph;
    Edge *leftEdge = nullptr;
    Edge *rightEdge = nullptr;
    bool dragging = false;
};


#endif //G14CONTROLPP_NODE_H
