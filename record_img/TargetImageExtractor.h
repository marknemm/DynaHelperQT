#ifndef TARGET_IMAGE_EXTRACTOR_H
#define TARGET_IMAGE_EXTRACTOR_H


#include <cv.h>
using namespace cv;


class TargetImageExtractor {

public:

	
	static cv::Rect iterativeExtractBasedInConstrast(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType,
													 int expansionRate=2, int baseW=10, int baseH=10);
	static cv::Rect extractBasedInContrast(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType);
	static cv::Rect extractBasedInMser(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType);

private:

	static cv::Rect getSmallestBoundRectAtLoc(const Mat &srcImg, const cv::Point &loc);

	static bool getNextIterationParams(const cv::Rect &srcImgRect, const cv::Point &globalLoc, cv::Point &roiLoc, cv::Rect &roi,
									   const cv::Rect &extractRect, int targetImgType, int expansionRate, int baseW, int baseH);
	static void checkForFixedRoiBounds(bool &fixX, bool &fixY, bool &fixX2, bool &fixY2, const cv::Rect &roi, const cv::Rect &extractRect);
	static void setRoiBoundsToFitFixed(bool fixX, bool fixY, bool fixX2, bool fixY2, const cv::Rect &oldRoi,
									   cv::Rect &roi, const cv::Rect &extractRect, cv::Point &roiLoc);

	static cv::Rect extractBoundsAtLocNoText(Mat &destImg, const Mat &srcImg, const cv::Point &loc, std::vector<cv::Rect> &boundsRects);
	static cv::Rect determineTarget(Mat &destImg, int &targetImgType, const Mat &textImg, const cv::Rect &textRect, const Mat &boundImg,
									cv::Rect &boundRect, const Mat &grayImg, int cursorType, const cv::Point &loc);
	static int getSidesBoundTightEnough(const cv::Rect &textRect, const cv::Rect &boundRect, bool &left, bool &top, bool &right, bool &bottom);

	TargetImageExtractor();
	TargetImageExtractor& operator=(const TargetImageExtractor &rhs);
};


#endif