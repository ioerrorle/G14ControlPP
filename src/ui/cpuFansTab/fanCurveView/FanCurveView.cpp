#include "FanCurveView.h"
#include "Edge.h"
#include "Node.h"
#include <QDebug>

FanCurveView::FanCurveView(QWidget *parent)
    : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setStyleSheet("border: 0px");

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    scene->setSceneRect(0, 0, width(), height());

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    for (int i = 0; i < 8; i++) {
        nodes[i] = new Node(this);
        scene->addItem(nodes[i]);
        if (i > 0)
            scene->addItem(new Edge(nodes[i - 1], nodes[i]));
    }

    uchar tempsTmp[] = {20 , 44 , 47 , 50 , 53 , 56 , 60 , 98};
    uchar speedsTmp[] = {7 , 11 , 14 , 16 , 19 , 23 , 28 , 35};

    mempcpy(&this->temps[0], tempsTmp, 8);
    mempcpy(&this->speeds[0], speedsTmp, 8);
    setNodesInPlace();
}

void FanCurveView::setFanCurve(const uchar *temps, const uchar *speeds) {
    mempcpy(&this->temps[0], temps, 8);
    mempcpy(&this->speeds[0], speeds, 8);
    setNodesInPlace();
}

void FanCurveView::getFanCurve(uchar *temps, uchar *speeds) {
    for (int i = 0; i < 8; i++) {
        temps[i] = this->temps[i];
        speeds[i] = this->speeds[i];
    }
}

void FanCurveView::itemMoved() {}

void FanCurveView::drawBackground(QPainter *painter, const QRectF &rect) {


    QRectF scene = this->sceneRect();

    const QFont &font = getAxisFont();
    painter->setFont(font);
    QFontMetrics fm(font);
    QString endTempText = QString::asprintf("%d°C", maxTemp);
    int endTempWidth = fm.horizontalAdvance(endTempText);
    int textHeight = fm.height();
    painter->drawText(scene.right() - endTempWidth, scene.bottom(), endTempText);
    rightGridBorder = scene.right() - endTempWidth / 2.;
    bottomGridBorder = scene.bottom() - textHeight - xAxisMargin;

    topGridBorder = scene.top() + textHeight / 2.;
    pixelPerPercent = (bottomGridBorder - topGridBorder) / 100;

    int maxNumOfSpeedStepsByTextSize = (bottomGridBorder - topGridBorder) / textHeight;
    int maxNumOfSpeedStepsByMaxSpeedStep = 100 / maxSpeedStep;

    int speedSteps = std::min(maxNumOfSpeedStepsByTextSize, maxNumOfSpeedStepsByMaxSpeedStep);
    qreal speedStep = 100. / speedSteps;

    leftGridBorder = 0;
    for (int i = 0; i < speedSteps; i++) {
        QString text = QString::asprintf("%d%%", 100 - (int)round(i * speedStep));
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
    painter->drawText(horizontalOffset, scene.bottom(), startTempText);

    //calculating offsets for horizontal scale
    int maxNumOfTempStepsByTextSize = (rightGridBorder - leftGridBorder) / endTempWidth;
    int maxNumOfTempStepsByMaxSpeedStep = (maxTemp - minTemp) / maxTempStep;
    int tempSteps = std::min(maxNumOfTempStepsByTextSize, maxNumOfTempStepsByMaxSpeedStep);
    qreal tempStep = ((double)maxTemp - minTemp) / tempSteps;

    for (int i = 1; i < tempSteps; i++) {
        QString text = QString::asprintf("%d°C", minTemp + (int)round(i * tempStep));
        int textWidth = fm.horizontalAdvance(text);
        painter->drawText(leftGridBorder + i * tempStep * pixelsPerDegree - textWidth / 2.,
                          scene.bottom(),
                          text);
    }

    //vertical lines
    for (int i = 0; i < tempSteps + 1; i++) {
        painter->drawLine(leftGridBorder + i * tempStep * pixelsPerDegree,
                          topGridBorder,
                          leftGridBorder + i * tempStep * pixelsPerDegree,
                          bottomGridBorder);
    }
//    if ((maxTemp - minTemp) % tempStep > 0) {
//        painter->drawLine(rightGridBorder, topGridBorder, rightGridBorder, bottomGridBorder);
//    }
}

void FanCurveView::resizeEvent(QResizeEvent *event) {
    qreal width = event->size().width();
    qreal height = event->size().height();
    setSceneRect(0, 0, width, height);
    calcBorders();
    setNodesInPlace();
}

QPointF FanCurveView::calcPosition(uchar temp, uchar speed) const {
    return QPointF(leftGridBorder + (temp - 20) * pixelsPerDegree,
                   bottomGridBorder - speed * pixelPerPercent);
}

QPoint FanCurveView::calcValues(QPointF &dotPosition) {
    return QPoint(qRound((dotPosition.x() - leftGridBorder) / pixelsPerDegree + 20),
                  qRound((dotPosition.y() - bottomGridBorder) / -pixelPerPercent));
}

void FanCurveView::setNodesInPlace() {
    for (int i = 0; i < 8; i++) {
        nodes[i]->setPos(calcPosition(temps[i], speeds[i]));
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

    int speedStep = maxSpeedStep < (100 * textHeight) / (bottomGridBorder - topGridBorder)  ? maxSpeedStep : (100 * textHeight) / (bottomGridBorder - topGridBorder);

    int speedSteps = 100 / speedStep;

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
