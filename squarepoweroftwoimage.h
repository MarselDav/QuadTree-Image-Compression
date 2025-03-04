#ifndef SQUAREPOWEROFTWOIMAGE_H
#define SQUAREPOWEROFTWOIMAGE_H

#include <QImage>
#include <QDebug>

class SquarePowerOfTwoImage
{
public:
    SquarePowerOfTwoImage(QString path);

    QSize getSquareImageSize();
    QPoint getQImageStartPoint();
    QImage *getQImage();

    QColor pixelColor(int x, int y);

    bool isNull();

private:
    QSize m_squareImageSize;
    QPoint m_QImageStartPoint;
    QImage *m_image;
};

#endif // SQUAREPOWEROFTWOIMAGE_H
