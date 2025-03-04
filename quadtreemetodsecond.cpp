#include "quadtreemetodsecond.h"
#include <iostream>

QuadTreeMetodSecond::QuadTreeMetodSecond()
{
    root = nullptr;
}

// Функция для вычисления евклидова расстояния между двумя цветами
double QuadTreeMetodSecond::calculateColorDistance(const QColor& color1, const QColor& color2) {
    double deltaR = color1.red() - color2.red();
    double deltaG = color1.green() - color2.green();
    double deltaB = color1.blue() - color2.blue();

    return std::sqrt(deltaR * deltaR + deltaG * deltaG + deltaB * deltaB);
}

// Функция для проверки приблизительной однородности 4 цветов
bool QuadTreeMetodSecond::areColorsApproximatelyEqual(const QColor& color1, const QColor& color2,
                                 const QColor& color3, const QColor& color4, const QColor& averageColor) {
    double distance1 = calculateColorDistance(color1, averageColor);
    double distance2 = calculateColorDistance(color2, averageColor);
    double distance3 = calculateColorDistance(color3, averageColor);
    double distance4 = calculateColorDistance(color4, averageColor);

    return (distance1 < THRESHOLD) && (distance2 < THRESHOLD) && (distance3 < THRESHOLD) && (distance4 < THRESHOLD);
}


bool QuadTreeMetodSecond::areColorsApproximatelyEqual(const QColor& color1, const QColor& color2)
{
    double distance = calculateColorDistance(color1, color2);

    return (distance < THRESHOLD);
}

int QuadTreeMetodSecond::calculateErrorBetweenImages(QImage image1, QImage image2)
{
    int error = 0;
    if (image1.size() == image2.size())
    {
        for (int y = 0; y < image1.size().height(); y++)
        {
            for (int x = 0; x < image1.size().width(); x++)
            {
                QColor pixelColor1 = image1.pixel(x, y);
                QColor pixelColor2 = image2.pixel(x, y);
                error += calculateColorDistance(pixelColor1, pixelColor2);
            }
        }
    }
    else
    {
        qDebug() << "Sizes is not equal";
    }
    return error;
}

void QuadTreeMetodSecond::loadImage(QString path)
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

    root = loadImageHelperMethodThree(28, 0, x, 0, y);
    //root = loadImageHelperMethodTwo(0, x, 0, y, image);
}



QuadTreeMetodSecond::Node* QuadTreeMetodSecond::loadImageHelper(int x, int width, int y, int height)
{
    Node* newNode = new Node();

    int red = 0;
    int green = 0;
    int blue = 0;

    if (width == 1 && height == 1)
    {
        QColor pixelColor = m_image->pixelColor(x, y);

        red = pixelColor.red();
        green = pixelColor.green();
        blue = pixelColor.blue();
    }
    else
    {
        Node* topLeft = loadImageHelper(x, width / 2, y, height / 2);
        Node* topRight = loadImageHelper(x + width / 2, width / 2, y, height / 2);
        Node* bottomLeft = loadImageHelper(x, width / 2, y + height / 2, height / 2);
        Node* bottomRight = loadImageHelper(x + width / 2, width / 2, y + height / 2, height / 2);

        QColor topLeftColor = topLeft->color;
        QColor topRightColor = topRight->color;
        QColor bottomLeftColor = bottomLeft->color;
        QColor bottomRightColor = bottomRight->color;

        red = (topLeftColor.red() + topRightColor.red() + bottomLeftColor.red() + bottomRightColor.red()) / 4;
        green = (topLeftColor.green() + topRightColor.green() + bottomLeftColor.green() + bottomRightColor.green()) / 4;
        blue = (topLeftColor.blue() + topRightColor.blue() + bottomLeftColor.blue() + bottomRightColor.blue()) / 4;

        QColor averageColor(red, green, blue);

        if (areColorsApproximatelyEqual(topLeftColor, topRightColor, bottomLeftColor, bottomRightColor, averageColor))
        {
            delete topLeft;
            delete topRight;
            delete bottomLeft;
            delete bottomRight;
        }
        else
        {
            newNode->topLeft = topLeft;
            newNode->topRight = topRight;
            newNode->bottomLeft = bottomLeft;
            newNode->bottomRight = bottomRight;
        }

    }

    newNode->setRGB(red, green, blue);
    return newNode;
}


QuadTreeMetodSecond::Node* QuadTreeMetodSecond::loadImageHelperMethodTwo(int x, int width, int y, int height)
{
    Node* newNode = new Node();

    int Red = 0;
    int Green = 0;
    int Blue = 0;

    int pixelsCnt = height * width;
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            QColor pixelColor = m_image->pixelColor(j, i);

            int red = pixelColor.red();
            int green = pixelColor.green();
            int blue = pixelColor.blue();

            Red += red;
            Green += green;
            Blue += blue;
        }
    }
    QColor averageColor(Red / pixelsCnt, Green / pixelsCnt, Blue / pixelsCnt);
    Red = Red / pixelsCnt;
    Green = Green / pixelsCnt;
    Blue = Blue / pixelsCnt;

    bool isEqual = true;
    //double error = 0;
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            QColor pixelColor = m_image->pixelColor(j, i);

            //error += calculateColorDistance(pixelColor,averageColor);
            isEqual = areColorsApproximatelyEqual(pixelColor, averageColor);
            if (!isEqual)
                break;
        }
        if (!isEqual)
            break;
    }
    //error = error / (width*height);
//    if (!isEqual)
    //qDebug() << error;
    if (!isEqual)
    {
        Node* topLeft = loadImageHelperMethodTwo(x, width / 2, y, height / 2);
        Node* topRight = loadImageHelperMethodTwo(x + width / 2, width / 2, y, height / 2);
        Node* bottomLeft = loadImageHelperMethodTwo(x, width / 2, y + height / 2, height / 2);
        Node* bottomRight = loadImageHelperMethodTwo(x + width / 2, width / 2, y + height / 2, height / 2);

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

QuadTreeMetodSecond::Node* QuadTreeMetodSecond::loadImageHelperMethodThree(int threshold, int x, int width, int y, int height)
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
    if (error > threshold)
    {
        Node* topLeft = loadImageHelperMethodThree(threshold, x, width / 2, y, height / 2);
        Node* topRight = loadImageHelperMethodThree(threshold, x + width / 2, width / 2, y, height / 2);
        Node* bottomLeft = loadImageHelperMethodThree(threshold, x, width / 2, y + height / 2, height / 2);
        Node* bottomRight = loadImageHelperMethodThree(threshold, x + width / 2, width / 2, y + height / 2, height / 2);

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

QImage QuadTreeMetodSecond::convertToImage()
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

bool QuadTreeMetodSecond::noChildren(Node *node)
{
    bool haveTopLeft = node->topLeft == nullptr;
    bool haveTopRight = node->topRight == nullptr;
    bool haveBottomLeft= node->bottomLeft == nullptr;
    bool haveBottomRight = node->bottomRight == nullptr;
    return haveTopLeft * haveTopRight * haveBottomLeft * haveBottomRight;
}

void QuadTreeMetodSecond::convertToImageHelper(int x, int width, int y, int height, QImage *image, Node* node)
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
        //image->setPixel(x, y, node->color.rgb());
    }
    else
    {
        convertToImageHelper(x, width/2, y, height/2, image, node->topLeft);
        convertToImageHelper(x + width/2, width/2, y, height/2, image, node->topRight);
        convertToImageHelper(x, width/2, y + height/2, height/2, image, node->bottomLeft);
        convertToImageHelper(x + width/2, width/2, y + height/2, height/2,image, node->bottomRight);
    }
}
