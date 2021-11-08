#include "FanCurveView.h"
#include "Node.h"
#include "Edge.h"
#include <QDebug>

FanCurveView::FanCurveView(QWidget *parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment( Qt::AlignTop | Qt::AlignLeft );
    setStyleSheet("border: 0px");


    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    scene->setSceneRect(0,0, width(), height());

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    
    uchar vals[] = {20 , 44 , 47 , 50 , 53 , 56 , 60 , 98 , 7 , 11 , 14 , 16 , 19 , 23 , 28 , 35};
    //uchar vals[] = {20 , 44 , 47 , 50 , 53 , 56 , 60 , 98 , 11 , 14 , 18 , 21 , 25 , 28 , 34 , 40 };


    for (int i = 0; i < 8; i++) {
        nodes[i] = new Node(this);
        scene->addItem(nodes[i]);
        if (i > 0)
            scene->addItem(new Edge(nodes[i-1], nodes[i]));
    }
}

void FanCurveView::setFanCurve(uchar *vals) {
    mempcpy(&values[0], vals, 16);
    setNodesInPlace();
}

void FanCurveView::itemMoved() {

}

void FanCurveView::drawBackground(QPainter *painter, const QRectF &rect) {
    int speedSteps = 100 / speedStep;

    QRectF scene = this->sceneRect();

    const QFont &font = getAxisFont();
    painter->setFont(font);
    QFontMetrics fm(font);
    QString endTempText = QString::asprintf("%d°C", maxTemp);
    int endTempWidth = fm.horizontalAdvance(endTempText);
    int textHeight = fm.height();
    painter->drawText(scene.right() - endTempWidth,
                      scene.bottom(),
                      endTempText);
    rightGridBorder = scene.right() - endTempWidth / 2.;
    bottomGridBorder = scene.bottom() - textHeight - xAxisMargin;

    topGridBorder = scene.top() + textHeight / 2.;
    pixelPerPercent = (bottomGridBorder - topGridBorder) / 100;

    leftGridBorder = 0;
    for (int i = 0; i < speedSteps; i++) {
        QString text = QString::asprintf("%d%%", 100 - i * speedStep);
        int textWidth = fm.horizontalAdvance(text);
        if (leftGridBorder < textWidth + yAxisMargin)
            leftGridBorder = textWidth + yAxisMargin;

        painter->drawText(scene.left(),
                          scene.top() + i * speedStep * pixelPerPercent + textHeight,
                          text);
    }

    leftGridBorder += scene.left();
    pixelsPerDegree = (rightGridBorder - leftGridBorder) / (maxTemp - minTemp);

    //horizontal lines
    for (int i = 0; i < speedSteps + 1; i++) {
        painter->drawLine(leftGridBorder,
                          i * speedStep * pixelPerPercent + topGridBorder,
                          rightGridBorder,
                          i * speedStep * pixelPerPercent + topGridBorder);
    }

    //horizontal scale
    //adding start value
    QString startTempText = QString::asprintf("%d°C", minTemp);
    int startTempWidth = fm.horizontalAdvance(startTempText);
    double horizontalOffset = leftGridBorder - startTempWidth / 2.;
    painter->drawText(horizontalOffset,
                      scene.bottom(),
                      startTempText);

    //calculating offsets for horizontal scale
    int steps = (maxTemp - minTemp) / tempStep;
    for (int i = 1; i < steps; i++) {
        QString text = QString::asprintf("%d°C", minTemp + i * tempStep);
        int textWidth = fm.horizontalAdvance(text);
        painter->drawText(leftGridBorder + i * tempStep * pixelsPerDegree - textWidth / 2.,
                          scene.bottom(),
                          text);
    }

    //vertical lines
    for (int i = 0; i < steps + 1; i++) {
        painter->drawLine(leftGridBorder + i * tempStep * pixelsPerDegree,
                          topGridBorder,
                          leftGridBorder + i * tempStep * pixelsPerDegree,
                          bottomGridBorder);

    }
    if ((maxTemp - minTemp) % tempStep > 0) {
        painter->drawLine(rightGridBorder,
                          topGridBorder,
                          rightGridBorder,
                          bottomGridBorder);
    }
}

void FanCurveView::resizeEvent(QResizeEvent *event) {
    qreal width =  event->size().width();
    qreal height = event->size().height();
    setSceneRect(0,0, width, height);
    calcBorders();
    setNodesInPlace();
}

QPointF FanCurveView::calcPosition(uchar temp, uchar speed) const {
    return QPointF(leftGridBorder + (temp - 20) * pixelsPerDegree, bottomGridBorder - speed * pixelPerPercent);
}

QPoint FanCurveView::calcValues(QPointF &dotPosition) {
    return QPoint(qRound((dotPosition.x() - leftGridBorder) / pixelsPerDegree + 20), qRound((dotPosition.y() - bottomGridBorder) / -pixelPerPercent));
}

void FanCurveView::setNodesInPlace() {
    for (int i = 0; i < 8; i++) {
        nodes[i]->setPos(calcPosition(values[i], values[i+8]));
    }
}

qreal FanCurveView::getRightGridBorder() const {
    return rightGridBorder;
}

qreal FanCurveView::getLeftGridBorder() const {
    return leftGridBorder;
}

qreal FanCurveView::getTopGridBorder() const {
    return topGridBorder;
}

qreal FanCurveView::getBottomGridBorder() const {
    return bottomGridBorder;
}

qreal FanCurveView::getPixelPerPercent() const {
    return pixelPerPercent;
}

qreal FanCurveView::getPixelsPerDegree() const {
    return pixelsPerDegree;
}

QFont FanCurveView::getAxisFont() {
    return font();
}

void FanCurveView::calcBorders() {
    int speedSteps = 100 / speedStep;

    QRectF scene = this->sceneRect();

    const QFont &font = getAxisFont();
    QFontMetrics fm(font);
    QString endTempText = QString::asprintf("%d°C", maxTemp);
    int endTempWidth = fm.horizontalAdvance(endTempText);
    int textHeight = fm.height();
    rightGridBorder = scene.right() - endTempWidth / 2.;
    bottomGridBorder = scene.bottom() - textHeight - xAxisMargin;

    topGridBorder = scene.top() + textHeight / 2.;
    pixelPerPercent = (bottomGridBorder - topGridBorder) / 100;

    leftGridBorder = 0;
    for (int i = 0; i < speedSteps; i++) {
        QString text = QString::asprintf("%d%%", 100 - i * speedStep);
        int textWidth = fm.horizontalAdvance(text);
        if (leftGridBorder < textWidth + yAxisMargin)
            leftGridBorder = textWidth + yAxisMargin;
    }

    leftGridBorder += scene.left();
    pixelsPerDegree = (rightGridBorder - leftGridBorder) / (maxTemp - minTemp);
}
