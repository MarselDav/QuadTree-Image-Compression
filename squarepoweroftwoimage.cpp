#include "squarepoweroftwoimage.h"

SquarePowerOfTwoImage::SquarePowerOfTwoImage(QString path)
{
    m_image = new QImage(path);

    QSize imageSize = m_image->size();

    int nearestWidth = pow(2, static_cast<int>(ceil(log2(imageSize.width()))));
    int nearestHeight = pow(2, static_cast<int>(ceil(log2(imageSize.height()))));

    int biggerValue = nearestWidth > nearestHeight ? nearestWidth : nearestHeight;
    m_squareImageSize = QSize(biggerValue, biggerValue);
    m_QImageStartPoint = QPoint((biggerValue - imageSize.width()) / 2,
                                (biggerValue - imageSize.width()) / 2);

}

QColor SquarePowerOfTwoImage::pixelColor(int x, int y)
{
    if (x >= m_QImageStartPoint.x() && x < m_QImageStartPoint.x() + m_image->size().width())
    {
        if (y >= m_QImageStartPoint.y() && y < m_QImageStartPoint.y() + m_image->size().height())
        {
            return m_image->pixelColor(x - m_QImageStartPoint.x(), y - m_QImageStartPoint.y());
        }
    }
    return nullptr;
}


QSize SquarePowerOfTwoImage::getSquareImageSize()
{
    return m_squareImageSize;
}

QImage * SquarePowerOfTwoImage::getQImage()
{
    return m_image;
}

QPoint SquarePowerOfTwoImage::getQImageStartPoint()
{
    return m_QImageStartPoint;
}

bool SquarePowerOfTwoImage::isNull()
{
    return m_image->isNull();
}
