#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include "quadtree.h"
#include <iostream>
#include <iomanip>


const QString IMAGE_700X700_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/700X700.jpg";
const QString SAVE_700X700_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/700X700_TREE";

const QString IMAGE_1024X768_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1024X768.jpg";
const QString SAVE_1024X768_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1024X768_TREE";

const QString IMAGE_1024X1024_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1024X1024.jpg";
const QString SAVE_1024X1024_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1024X1024_TREE";

const QString IMAGE_1920X1200_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1920X1200.jpg";
const QString SAVE_1920X1200_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/1920X1200_TREE";

const QString IMAGE_2048X2048_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/2048X2048.jpg";
const QString SAVE_2048X2048_PATH = "C:/Users/legion/Documents/AISD_KURSOVAYA/2048X2048_TREE";

const int PIXEL_SIZE_IN_BYTES = 3;

int getImageSizeInBytes(QImage image)
{
    QSize size = image.size();

    return size.width() * size.height() * PIXEL_SIZE_IN_BYTES;
}


int calculateErrorBetweenImages(QImage image1, QImage image2)
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
                error += QuadTree::calculateColorDistance(pixelColor1, pixelColor2);
            }
        }
    }
    else
    {
        qDebug() << "Sizes is not equal";
    }
    return error;
}

const int MAX_THRESHOLD = 150;

void getResultForGraphic(QString path)
{
    QImage originalImage(path);

    std::cout << "y = [";
    for (int threshold = 0; threshold < MAX_THRESHOLD; threshold++)
    {
        QuadTree quadTree;
        quadTree.setThreshold(threshold);

        quadTree.loadImage(path);
        QImage image = quadTree.convertToImage();

        int error = calculateErrorBetweenImages(originalImage, image);
        std::cout << error << ", ";
    }
    std::cout << "]" << std::endl;
}


void getResultInPercentagesForGraphic(QString path)
{
    QImage originalImage(path);

    float fileOrigSize = getImageSizeInBytes(originalImage);

    std::cout << "y = [";
    for (int threshold = 0; threshold < MAX_THRESHOLD; threshold++)
    {
        QuadTree quadTree;
        quadTree.setThreshold(threshold);
        quadTree.loadImage(path);
        float fileSize = quadTree.getImageSizeInBytes();

        float perc = (fileSize / fileOrigSize) * 100;
        std::cout << perc << ", ";

    }
    std::cout << "]" << std::endl;
}

void getAbsoluteSizeInKbForGraphic(QString path)
{
    QImage originalImage(path);

    std::cout << "y = [";
    for (int threshold = 0; threshold < MAX_THRESHOLD; threshold++)
    {
        QuadTree quadTree;
        quadTree.setThreshold(threshold);
        quadTree.loadImage(path);
        float fileSize = float(quadTree.getImageSizeInBytes()) / 1024;
        std::cout << fileSize << ", ";

    }
    std::cout << "]" << std::endl;
}

//const int THRESHOLD = 30;

void loadImageToTree(int threshold, QString path, QString pathSave)
{
    QuadTree quadTree;
    quadTree.setThreshold(threshold);

    quadTree.loadImage(path);
    QImage image = quadTree.convertToImage();

    image.save(pathSave + QString("_%1.jpg").arg(threshold));
}

const int START_THRESHOLD = 20;
const int END_THRESHOLD = 60;
const int STEP = 10;

void loadImageToTreeWithStepThreshold(QString path, QString pathSave)
{
    for (int threshold = START_THRESHOLD; threshold < END_THRESHOLD; threshold+=STEP)
    {
        loadImageToTree(threshold, path, pathSave);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString PATH = IMAGE_2048X2048_PATH;
    QString PATH_SAVE = SAVE_2048X2048_PATH;
    const int THRESHOLD = 50;

    //getResultInPercentagesForGraphic(PATH);
    //getResultForGraphic(PATH);

    QImage originalImage(PATH);
    QuadTree quadTree;
    quadTree.setThreshold(THRESHOLD);

    quadTree.loadImage(PATH);
    QImage image = quadTree.convertToImage();

    return 1;
}
