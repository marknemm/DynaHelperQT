#ifndef RECORD_IMG_DBG_H
#define RECORD_IMG_DBG_H


#include <cv.h>
#include <QRect>
using namespace cv;


class RecordImgDbg {

public:

    /**
     * @brief displayImg
     * Displays a given image in a window with a given name.
     * @param img
     * The Image to display.
     * @param name (OPTIONAL)
     * The name of the display window.
     */
	static void displayImg(const Mat &img, const char *name=nullptr);


    /**
     * @brief printRect
     * Prints the details of a given rectangle.
     * @param rect
     * The rectangle to print the details of.
     */
    static void printRect(const cv::Rect &rect);

    /**
     * @brief printRect
     * Prints the details of a given rectangle.
     * @param rect
     * The rectangle to print the details of.
     */
    static void printRect(const QRect &rect);


private:

    // Private default constructor and assignment operator. Should never be instantiated (all methods are static).
    RecordImgDbg();
    RecordImgDbg& operator=(const RecordImgDbg &rhs);
};


#endif
