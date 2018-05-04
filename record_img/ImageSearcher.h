#ifndef IMAGE_SEARCHER_H
#define IMAGE_SEARCHER_H


#include <cv.h>
using namespace cv;

/**
--- DESCRIPTION ---
Toolkit for image searching functionalities.
--- AUTHOR ---
Mark Nemmer
*/
class ImageSearcher {

public:

	static cv::Point templateMatchSearch(const Mat &targetImg, const Mat &screenshot, const cv::Point &targetOrgPredict, int matchMethod=CV_TM_CCOEFF);
	static cv::Point featureBasedSearch(const Mat &targetImg, const Mat &screenshot, const cv::Point &targetOrgPredict);

private:

	static cv::Rect clacRoiAroundTargetCenter(const cv::Point &targetOrgPredict, const Mat &targetImg, const Mat &screenshot, int roiScale=3);
};


#endif