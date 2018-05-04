#include "RecordImgDbg.h"
#include "NativeImageUtil.h"
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>


//#define #DEBUG_ON


void RecordImgDbg::displayImg(const Mat &img, const char *name /*=nullptr*/)
{
    #ifdef DEBUG_ON
        Mat dispImg;
        if (name == nullptr) name = "Debug Image";
        NativeImageUtil::fitImgToScreen(img, dispImg);
        namedWindow(name, WINDOW_AUTOSIZE);
        imshow(name, dispImg);
        moveWindow(name, 0, 0);
        waitKey(0);
        cvDestroyWindow(name);
    #endif
}


void RecordImgDbg::printRect(const cv::Rect &rect)
{
    qDebug() << "X: " << rect.x;
    qDebug() << "Y: " << rect.y;
    qDebug() << "W: " << rect.width;
    qDebug() << "H: " << rect.height;
}


void RecordImgDbg::printRect(const QRect &rect)
{
    qDebug() << "X: " << rect.x();
    qDebug() << "Y: " << rect.y();
    qDebug() << "W: " << rect.width();
    qDebug() << "H: " << rect.height();
}
