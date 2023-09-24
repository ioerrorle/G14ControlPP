#include "node.h"
#include "edge.h"
#include "fanCurveWidget.h"
#include <QDebug>
#include <QToolTip>

Node::Node(FanCurveView *fanCurveView)
    : graph(fanCurveView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && dragging) {
        //CPU: at least 28% at 60C, 35% at 98C
        //GPU: at least 35% at 60C, 40% at 98C
        bool fixed = false;
        QPointF newPos = value.toPointF();
        qreal minX = graph->getLeftGridBorder();;
        qreal maxX = graph->getRightGridBorder();
        qreal minY = graph->getTopGridBorder();
        qreal maxY = graph->getBottomGridBorder();
        if (leftEdge != nullptr) {
            minX = leftEdge->sourceNode()->pos().x() + graph->getPixelsPerDegree();
        }
        if (rightEdge != nullptr) {
            maxX = rightEdge->destNode()->pos().x() - graph->getPixelsPerDegree();
        }

        if (newPos.x() < minX) {
            fixed = true;
            newPos.setX(minX);
        }
        if (newPos.x() > maxX) {
            fixed = true;
            newPos.setX(maxX);
        }
        if (newPos.y() < minY) {
            fixed = true;
            newPos.setY(minY);
        }
        if (newPos.y() > maxY) {
            fixed = true;
            newPos.setY(maxY);
        }

        QPoint values = graph->calcValues(newPos);
        newPos = graph->calcPosition(values.x(), values.y());
        QToolTip::showText(QCursor::pos(), QString::asprintf("%d°C, %d%%", values.x(), values.y()), nullptr);
        return newPos;
    }

    if (change == ItemPositionHasChanged) {
        if (leftEdge != nullptr)
            leftEdge->adjust();
        if (rightEdge != nullptr)
            rightEdge->adjust();
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    dragging = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    dragging = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::setLeftEdge(Edge *left) {
    leftEdge = left;
}

void Node::setRightEdge(Edge *right) {
    rightEdge = right;
}

Edge *Node::getLeftEdge() const {
    return leftEdge;
}

Edge *Node::getRightEdge() const {
    return rightEdge;
}

bool Node::advancePosition() {
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const {
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust, 20 + adjust, 20 + adjust);
}

QPainterPath Node::shape() const {
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    painter->setBrush(QColor(Qt::red));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(-5, -5, 10, 10);
}
