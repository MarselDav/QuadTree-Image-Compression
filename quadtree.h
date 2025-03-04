#ifndef QUADTREE_H
#define QUADTREE_H

#include <QString>
#include <QImage>
#include <QDebug>
#include <QColor>
#include <cmath>
#include "squarepoweroftwoimage.h"

class QuadTree
{
public:
    QuadTree();

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

    // Функция для вычисления евклидова расстояния между двумя цветами
    static double calculateColorDistance(const QColor& color1, const QColor& color2);

    void setThreshold(double Threshold);

    void loadImage(QString path);
    Node* loadImageHelper(int x, int width, int y, int height);

    QImage convertToImage();
    void convertToImageHelper(int x, int width, int y, int height, QImage *image, Node* node);

    int getImageSizeInBytes();
    int getImageSizeInBytesHelper(Node* node);

private:
    const int PIXEL_SIZE_IN_BYTES = 3;

    SquarePowerOfTwoImage *m_image;
    double THRESHOLD = 500;  // Порог для определения схожести цветов
    Node *root;

    bool areColorsApproximatelyEqual(const QColor& color1, const QColor& color2);

    bool noChildren(Node *node);
};

#endif // QUADTREE_H
