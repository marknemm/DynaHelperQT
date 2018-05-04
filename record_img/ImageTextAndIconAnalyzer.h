#ifndef IMAGE_TEXT_AND_ICON_ANALYZER_H
#define IMAGE_TEXT_AND_ICON_ANALYZER_H


#include <cv.h>
#include "ImageConverter.h"
using namespace cv;


/**
--- DESCRIPTION ---
Static utility class used for analyzing text and icons in an image. Contains functionality to find and extract text and icons.
*/
class ImageTextAndIconAnalyzer {

public:

	static cv::Rect extractTextAndIconsAtLoc(Mat &destImg, const Mat &srcImg, const cv::Point &loc,
											 std::vector<cv::Rect> &textBoundingRects, std::vector<cv::Rect> &iconBoundingRects);
	static void filterOutTextOrIcons(Mat &destImg, const Mat &srcImg, const std::vector<cv::Rect> *textBoundingRects=nullptr,
									 const std::vector<cv::Rect> *iconBoundingRects=nullptr);

private:

	static void detectTextAndIcons(const Mat &img, std::vector<cv::Rect> &textBoundingRects, std::vector<cv::Rect> &iconBoundingRects);
	static bool isTextOrIcon(const Mat &bwImg, const cv::Rect &boundingRect, bool &isText);
	static int scanLineForIntensityChangeCount(const Mat &scanImg, int x1, int x2, int y1, int y2, int changeLim=1000000);

	// Make private because class is static only!
	ImageTextAndIconAnalyzer();
	ImageTextAndIconAnalyzer& operator=(const ImageTextAndIconAnalyzer &rhs);
};


#endif