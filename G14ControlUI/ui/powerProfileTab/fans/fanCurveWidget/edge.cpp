#include "edge.h"
#include "node.h"

#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode)
        : source(sourceNode), dest(destNode) {
    setAcceptedMouseButtons(Qt::NoButton);
    source->setRightEdge(this);
    dest->setLeftEdge(this);
    adjust();
}

Node *Edge::sourceNode() const {
    return source;
}

Node *Edge::destNode() const {
    return dest;
}

void Edge::adjust() {
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();
    //QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
    sourcePoint = line.p1();// + edgeOffset;
    destPoint = line.p2();// - edgeOffset;
}

QRectF Edge::boundingRect() const {
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
