#ifndef CONTRAST_BASED_PREPROCESSING_H
#define CONTRAST_BASED_PREPROCESSING_H


#include <cv.h>


/**
--- DESCRIPTION ---
Static utility class used for preprocessing of an image.
The preprocessing is primarily based off of the property of contrast in a grayscale image.
*/
class ImagePreprocessing {

public:

	static void contrastBasedSeparateForegroundFromBackground(cv::Mat &destImg, const cv::Mat &srcImg);
    static void connectHorizontallyCloseObjects(cv::Mat &destImg, const cv::Mat &srcImg, int connectionWidth=9);
    static void removeLooseHorizontalConnections(cv::Mat &destImg, const cv::Mat &srcImg, int connectionHeight=3);

private:

	ImagePreprocessing();
	ImagePreprocessing& operator=(const ImagePreprocessing &rhs);
};


#endif
