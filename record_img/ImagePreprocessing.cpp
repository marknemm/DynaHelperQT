#include "ImagePreprocessing.h"
#include "ImageConverter.h"


/**
--- DESCRIPTION ---
Separates the foreground of an image from the background based off of contrast.
--- INPUTS ---
destImg: The destination image.
srcImg: The source image.
*/
void ImagePreprocessing::contrastBasedSeparateForegroundFromBackground(Mat &destImg, const Mat &srcImg)
{
	Mat morphKernel, grayImg;
    grayImg = ImageConverter::convertToGrayIfOtherFormat(srcImg);
    morphKernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(3, 3));
    morphologyEx(grayImg, destImg, MORPH_GRADIENT, morphKernel);
}


/**
--- DESCRIPTION ---
Connects horizontally close objects in a grayscale image.
--- INPUTS ---
destImg: The destination image.
srcImg: The source image.
*/
void ImagePreprocessing::connectHorizontallyCloseObjects(Mat &destImg, const Mat &srcImg, int connectWidth /*=9*/)
{
	Mat imgGray, morphKernel;
    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);
	morphKernel = getStructuringElement(MORPH_RECT, cv::Size(connectWidth, 1));
    morphologyEx(imgGray, destImg, MORPH_CLOSE, morphKernel);
}


/**
--- DESCRIPTION ---
Removes loosely horizontally connected objects within an image. This may be useful after calling connectHorizontallyCloseObjects() if
too many connections were formed.
--- INPUTS ---
destImg: The destination image.
srcImg: The source image.
connectionHeight: (OPTIONAL default=3) The height of the horizontal connection segments that loosely connect two objects which are to be removed.
*/
void ImagePreprocessing::removeLooseHorizontalConnections(Mat &destImg, const Mat &srcImg, int connectionHeight /*=3*/)
{
	Mat imgGray, morphKernel;
    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);
	morphKernel = getStructuringElement(MORPH_RECT, cv::Size(1, connectionHeight));
	morphologyEx(srcImg, destImg, MORPH_OPEN, morphKernel);
}


// Private do-nothing constructor & assignment op because static only!
ImagePreprocessing::ImagePreprocessing(){}
ImagePreprocessing& ImagePreprocessing::operator=(const ImagePreprocessing &rhs) { return *this; }
