#ifndef G14CONTROLPP_FANCURVEVIEW_H
#define G14CONTROLPP_FANCURVEVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class Node;

class FanCurveView : public QGraphicsView {

Q_OBJECT

public:
    FanCurveView(QWidget *parent = nullptr);

    void itemMoved();

    qreal getRightGridBorder() const;

    qreal getLeftGridBorder() const;

    qreal getTopGridBorder() const;

    qreal getBottomGridBorder() const;

    qreal getPixelPerPercent() const;

    qreal getPixelsPerDegree() const;

    QPoint calcValues(QPointF &dotPosition);

    QPointF calcPosition(uchar temp, uchar speed) const;

    void setFanCurve(const uchar *temps, const uchar *speeds);

    void getFanCurve(uchar *temps, uchar *speeds);
protected:
    void resizeEvent(QResizeEvent *event) override;

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    int minTemp = 20;
    int maxTemp = 105;
    int maxSpeedStep = 10;
    int maxTempStep = 5;
    int xAxisMargin = 2;
    int yAxisMargin = 2;

    Node *nodes[8];

    qreal rightGridBorder;
    qreal leftGridBorder;
    qreal topGridBorder;
    qreal bottomGridBorder;
    qreal pixelPerPercent;
    qreal pixelsPerDegree;

    uchar temps[8];
    uchar speeds[8];

    void setNodesInPlace();

    void calcBorders();

    QFont getAxisFont();
};


#endif //G14CONTROLPP_FANCURVEVIEW_H
