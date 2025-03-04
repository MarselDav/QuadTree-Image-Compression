#ifndef QUADTREEMETODSECOND_H
#define QUADTREEMETODSECOND_H

#include <QString>
#include <QImage>
#include <QDebug>
#include <QColor>
#include <cmath>
#include "squarepoweroftwoimage.h"

class QuadTreeMetodSecond
{
public:
    QuadTreeMetodSecond();

    struct Node
    {
        Node *topLeft;
        Node *topRight;
        Node *bottomRight;
        Node *bottomLeft;

        QColor color;
        Node(int R = 0, int G = 0, int B = 0)
        {
            color.setRgb(R, G, B);

            topLeft = nullptr;
            topRight = nullptr;
            bottomRight = nullptr;
            bottomLeft = nullptr;
        }

        void setRGB(int R, int G, int B)
        {
            color.setRgb(R, G, B);
        }

        int Red()
        {
            return color.red();
        }

        int Green()
        {
            return color.green();
        }

        int Blue()
        {
            return color.blue();
        }
    };

    void loadImage(QString path);
    Node* loadImageHelper(int x, int width, int y, int height);
    Node* loadImageHelperMethodTwo(int x, int width, int y, int height);
    int GeneralError = 0;
    Node* loadImageHelperMethodThree(int threshold, int x, int width, int y, int height);
    int calculateErrorBetweenImages(QImage image1, QImage image2);

    QImage convertToImage();
    void convertToImageHelper(int x, int width, int y, int height, QImage *image, Node* node);

private:
    SquarePowerOfTwoImage *m_image;
//    const double THRESHOLD = 210.0;  // Порог для определения схожести цветов
    double THRESHOLD = 500;  // Порог для определения схожести цветов
    Node *root;

    bool areColorsApproximatelyEqual(const QColor& color1, const QColor& color2);
    // Функция для вычисления евклидова расстояния между двумя цветами
    double calculateColorDistance(const QColor& color1, const QColor& color2);
    // Функция для проверки приблизительной однородности 4 цветов
    bool areColorsApproximatelyEqual(const QColor& color1, const QColor& color2, const QColor& color3, const QColor& color4, const QColor& averageColor);

    bool noChildren(Node *node);
};

#endif // QUADTREEMETODSECOND_H
