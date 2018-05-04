#include "NativeImageUtil.h"
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <qDebug>


QImage NativeImageUtil::takeScreenshot()
{
    QPixmap shot;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen != nullptr) {
       shot = screen->grabWindow(0);
    }
    return shot.toImage();
}


/**
Gets the bounding rectangle for the screen.
--- INPUT ---
screenBound: Will be set to the bounding rectangle.
*/
void NativeImageUtil::getScreenBound(cv::Rect &screenBound)
{
    QRect qScreenBound = QApplication::desktop()->screenGeometry();
    copyQRECTtoCvRect(screenBound, qScreenBound);
}


/**
--- DESCRIPTION ---
Copies the location and size of a native Windows RECT structure to an OpenCV Rect object.
--- INPUTS ---
cvRect: The destination of the copy.
winRect: The source of the copy.
*/
void NativeImageUtil::copyQRECTtoCvRect(cv::Rect &cvRect, const QRect &qRect)
{
    cvRect.x = qRect.x();
    cvRect.y = qRect.y();
    cvRect.width = qRect.width();
    cvRect.height = qRect.height();
}


void NativeImageUtil::copyCvRectToQRECT(QRect &qRect, const cv::Rect &cvRect)
{
    qRect.setX(cvRect.x);
    qRect.setY(cvRect.y);
    qRect.setWidth(cvRect.width);
    qRect.setHeight(cvRect.height);
}


void NativeImageUtil::copyQPointToCvPoint(cv::Point &cvPoint, const QPoint &qPoint)
{
    cvPoint.x = qPoint.x();
    cvPoint.y = qPoint.y();
}


void NativeImageUtil::copyCvPointToQPoint(QPoint &qPoint, const cv::Point &cvPoint)
{
    qPoint.setX(cvPoint.x);
    qPoint.setY(cvPoint.y);
}


/**
--- DESCRIPTION ---
Fits a given (Mat) Image to the sceen if it is too large.
--- INPUTS ---
img: The image to fit to the screen (if too large).
fitImg: The fitted version of the image (same as img if already small enought for screen size).
*/
void NativeImageUtil::fitImgToScreen(const cv::Mat &img, cv::Mat &fitImg)
{
	int widthImg, heightImg;
	cv::Rect screenSize;
	getScreenBound(screenSize);
	if(screenSize.width < img.cols) {
		widthImg = screenSize.width;
	}
	else { widthImg = img.cols; }
	if((screenSize.height - 80) < img.rows) {
		heightImg = (screenSize.height - 80);
	}
	else { heightImg = img.rows; }
	resize(img, fitImg, cv::Size(widthImg, heightImg));
}
