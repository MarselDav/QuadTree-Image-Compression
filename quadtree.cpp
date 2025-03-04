#include "quadtree.h"
#include <iostream>

QuadTree::QuadTree()
{
    root = nullptr;
}

// Функция для вычисления евклидова расстояния между двумя цветами
double QuadTree::calculateColorDistance(const QColor& color1, const QColor& color2) {
    double deltaR = color1.red() - color2.red();
    double deltaG = color1.green() - color2.green();
    double deltaB = color1.blue() - color2.blue();

    return std::sqrt(deltaR * deltaR + deltaG * deltaG + deltaB * deltaB);
}

bool QuadTree::areColorsApproximatelyEqual(const QColor& color1, const QColor& color2)
{
    double distance = calculateColorDistance(color1, color2);

    return (distance < THRESHOLD);
}

void QuadTree::loadImage(QString path)
{
    m_image = new SquarePowerOfTwoImage(path);

    // Проверка на успешную загрузку
    if (m_image->isNull()) {
        qDebug() << "Failed to load image";
        return;
    }

    QSize size = m_image->getSquareImageSize();
    int x = size.width();
    int y = size.height();

    root = loadImageHelper(0, x, 0, y);
}


QuadTree::Node* QuadTree::loadImageHelper(int x, int width, int y, int height)
{
    Node* newNode = new Node();

    int Red = 0;
    int Green = 0;
    int Blue = 0;

    int pixelsCnt = 0;
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            QColor pixelColor = m_image->pixelColor(j, i);
            if (pixelColor != nullptr)
            {
                int red = pixelColor.red();;
                int green = pixelColor.green();
                int blue = pixelColor.blue();

                Red += red;
                Green += green;
                Blue += blue;
                pixelsCnt++;
            }
        }
    }

    QColor averageColor(0, 0, 0);
    if (pixelsCnt != 0)
    {
        averageColor = QColor (Red / pixelsCnt, Green / pixelsCnt, Blue / pixelsCnt);
        Red = Red / pixelsCnt;
        Green = Green / pixelsCnt;
        Blue = Blue / pixelsCnt;
    }

    double error = 0;
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            QColor pixelColor = m_image->pixelColor(j, i);
            if (pixelColor != nullptr)
            {
                error += calculateColorDistance(pixelColor, averageColor);
            }
        }
    }
    error = error / pixelsCnt;
    if (error > THRESHOLD)
    {
        Node* topLeft = loadImageHelper(x, width / 2, y, height / 2);
        Node* topRight = loadImageHelper(x + width / 2, width / 2, y, height / 2);
        Node* bottomLeft = loadImageHelper(x, width / 2, y + height / 2, height / 2);
        Node* bottomRight = loadImageHelper(x + width / 2, width / 2, y + height / 2, height / 2);

        QColor topLeftColor = topLeft->color;
        QColor topRightColor = topRight->color;
        QColor bottomLeftColor = bottomLeft->color;
        QColor bottomRightColor = bottomRight->color;

        Red = (topLeftColor.red() + topRightColor.red() + bottomLeftColor.red() + bottomRightColor.red()) / 4;
        Green = (topLeftColor.green() + topRightColor.green() + bottomLeftColor.green() + bottomRightColor.green()) / 4;
        Blue = (topLeftColor.blue() + topRightColor.blue() + bottomLeftColor.blue() + bottomRightColor.blue()) / 4;

        QColor averageColor(Red, Green, Blue);

        newNode->topLeft = topLeft;
        newNode->topRight = topRight;
        newNode->bottomLeft = bottomLeft;
        newNode->bottomRight = bottomRight;
    }

    newNode->setRGB(Red, Green, Blue);
    return newNode;
}

QImage QuadTree::convertToImage()
{
    int width = m_image->getSquareImageSize().width();
    int height = m_image->getSquareImageSize().width();

    QImage *image = new QImage(width, height, QImage::Format_RGB888);
    convertToImageHelper(0, width, 0, height, image, root);

    QPoint startPoint = m_image->getQImageStartPoint();
    QSize originalImageSize = m_image->getQImage()->size();
    QImage newImage = image->copy(startPoint.x(), startPoint.y(), originalImageSize.width(), originalImageSize.height());
    return newImage;
}

bool QuadTree::noChildren(Node *node)
{
    bool haveTopLeft = node->topLeft == nullptr;
    bool haveTopRight = node->topRight == nullptr;
    bool haveBottomLeft= node->bottomLeft == nullptr;
    bool haveBottomRight = node->bottomRight == nullptr;
    return haveTopLeft * haveTopRight * haveBottomLeft * haveBottomRight;
}

void QuadTree::convertToImageHelper(int x, int width, int y, int height, QImage *image, Node* node)
{
    if (noChildren(node))
    {
        if (width != 1 && height != 1)
        {
            for (int i = y; i < y + height; i++)
            {
                for (int j = x; j < x + width; j++)
                {
                    image->setPixel(j, i, node->color.rgb());
                }
            }
        }
        else
        {
            image->setPixel(x, y, node->color.rgb());
        }
    }
    else
    {
        convertToImageHelper(x, width/2, y, height/2, image, node->topLeft);
        convertToImageHelper(x + width/2, width/2, y, height/2, image, node->topRight);
        convertToImageHelper(x, width/2, y + height/2, height/2, image, node->bottomLeft);
        convertToImageHelper(x + width/2, width/2, y + height/2, height/2,image, node->bottomRight);
    }
}

int QuadTree::getImageSizeInBytes()
{
    return getImageSizeInBytesHelper(root);
}

int QuadTree::getImageSizeInBytesHelper(Node* node)
{
    if (noChildren(node))
    {
        return PIXEL_SIZE_IN_BYTES;
    }
    else
    {
        int sizeTopLeft = getImageSizeInBytesHelper(node->topLeft);
        int sizeTopRight = getImageSizeInBytesHelper(node->topRight);
        int sizeBottomLeft = getImageSizeInBytesHelper(node->bottomLeft);
        int sizeBottomRight = getImageSizeInBytesHelper(node->bottomRight);

        return  sizeTopLeft + sizeTopRight + sizeBottomLeft + sizeBottomRight;
    }
}

void QuadTree::setThreshold(double Threshold)
{
    THRESHOLD = Threshold;
}
