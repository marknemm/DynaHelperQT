#ifndef NATIVE_IMAGE_UTIL_H
#define NATIVE_IMAGE_UTIL_H


#include <string>
#include <QImage>
#include <QRect>
#include <cv.h>


class NativeImageUtil {

public:

    /**
     * @brief takeScreenshot
     * Takes a screenshot using Qt Utilities.
     * @return The screenshot.
     */
    static QImage takeScreenshot();
	static void getScreenBound(cv::Rect &screenSize);
    static void copyQRECTtoCvRect(cv::Rect &cvRect, const QRect &qRect);
    static void copyCvRectToQRECT(QRect &qRect, const cv::Rect &cvRect);
    static void copyQPointToCvPoint(cv::Point &cvPoint, const QPoint &qPoint);
    static void copyCvPointToQPoint(QPoint &qPoint, const cv::Point &cvPoint);
	static void fitImgToScreen(const cv::Mat &img, cv::Mat &fitImg);
};


#endif
