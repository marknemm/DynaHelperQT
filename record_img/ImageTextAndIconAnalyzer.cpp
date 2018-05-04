#include "ImageTextAndIconAnalyzer.h"
#include "ImagePreprocessing.h"
#include "BoundsChecker.h"
#include "RecordImgDbg.h"
#include <QDebug>

/**
--- DESCRIPTION ---
Finds all text and small icons in the (grayscale) screenshot image related to the triggering action. Then, it extracts any found text
local to the triggering action.
--- INPUTS ---
destImg: A reference to a Mat object that will be set to the text or small icon local to the triggering action.
srcImg: A const reference to a Mat object that should contain the screenshot image related to the triggering action (in grayscale).
loc: A const reference to a Point object that should contain the screen coordinates of the triggering action.
textBoundingRects: A vector of rect objects that will be filled with all of the bounding rectangles of found text in the srcImg.
iconBoundingRects: A vector of rect objects that will be filled with all of the bounding rectangles of found icons in the srcImg.
--- OUTPUT ---
A rectangle object that contains the bounds around the extracted text image local to the triggering action.
*/
cv::Rect ImageTextAndIconAnalyzer::extractTextAndIconsAtLoc(Mat &destImg, const Mat &srcImg, const cv::Point &loc,
													        std::vector<cv::Rect> &textBoundingRects, std::vector<cv::Rect> &iconBoundingRects)
{
	cv::Rect tightestBoundRect, tempRect;

	// Detect all text and icons, then find the tightest bounding text/icon rectangle around target loc.
	detectTextAndIcons(srcImg, textBoundingRects, iconBoundingRects);
	tightestBoundRect = BoundsChecker::getSmallestBoundingRect(textBoundingRects, loc);
	tempRect = BoundsChecker::getSmallestBoundingRect(iconBoundingRects, loc);
	if(BoundsChecker::isRectSmaller(tempRect, tightestBoundRect)) {
		tightestBoundRect = tempRect;
	}

	// If we have found a tightest bounding text rect that holds our given loc:
	if(tightestBoundRect.x >= 0) {
		destImg = srcImg(cv::Range(tightestBoundRect.y, tightestBoundRect.y + tightestBoundRect.height),
						 cv::Range(tightestBoundRect.x, tightestBoundRect.x + tightestBoundRect.width));
	}

	return tightestBoundRect;
}


Mat imgTest;


/**
--- DESCRIPTION ---
Detects all text and icons in the (grayscale) screenshot image related to the triggering action.
--- INPUTS ---
img: A const reference to a Mat object that should contain the screenshot image related to the triggering action (in grayscale).
textAndIconsBoundingRects: A reference to a vector of rect objects that will be filled with all of the bounding rectangles of found text
						   and icons in the srcImg.
*/
void ImageTextAndIconAnalyzer::detectTextAndIcons(const cv::Mat &img, std::vector<cv::Rect> &textBoundingRects, std::vector<cv::Rect> &iconBoundingRects)
{
	Mat imgGray, imgContrast, imgBW, imgConnected, imgFilledContours, imgFilledContoursTest;
	std::vector<std::vector<cv::Point>> contours;
    std::vector<Vec4i> hierarchy;
	bool isText;

	// Image preprocessing.
    imgGray = ImageConverter::convertToGrayIfOtherFormat(img);
	ImagePreprocessing::contrastBasedSeparateForegroundFromBackground(imgContrast, imgGray);
    imgBW = ImageConverter::convertGrayToBW(imgContrast);
    ImagePreprocessing::connectHorizontallyCloseObjects(imgConnected, imgBW);
	//ImagePreprocessing::removeLooseHorizontalConnections(imgConnected, imgConnected);
    findContours(imgConnected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	imgFilledContours = Mat::zeros(imgConnected.size(), CV_8UC1);
	drawContours(imgFilledContours, contours, -1/*Draw all contours at once.*/, Scalar(255), CV_FILLED);
	
    imgTest = ImageConverter::convertGrayToRGB(imgBW);
    imgFilledContoursTest = ImageConverter::convertGrayToRGB(imgFilledContours);

	// filter out non-text or icon contours.
	if(contours.size() > 0) {
		for(int idx = 0 ; idx >= 0 ; idx = hierarchy[idx][0]) {
			cv::Rect rect = boundingRect(contours[idx]);
			if(isTextOrIcon(imgBW, rect, isText)) {
				if(isText) {
					rectangle(imgFilledContoursTest, rect, Scalar(0, 0, 255), 3);
					textBoundingRects.push_back(rect);
				}
				else {
					rectangle(imgFilledContoursTest, rect, Scalar(255, 0, 0), 3);
					iconBoundingRects.push_back(rect);
				}
			}
		}
	}

	RecordImgDbg::displayImg(imgBW, "Scan Image");
	RecordImgDbg::displayImg(imgTest, "Post Text & Icon Analysis");
	RecordImgDbg::displayImg(imgFilledContoursTest, "Detected Text (Red) & Detected Icons (Blue)");
}


/**
--- DESCRIPTION ---
Determines if a region within an image contains text or a small icon.
--- INPUTS ---
imgBW: A const reference to a Mat object containing the (black & white) image to check a bounding region for a text or icon.
boundingRect: A const refrence to the rectangle that describes the bounding region to check within imgBW.
--- OUTPUT ---
A BOOL containing TRUE if the region does contain text or an icon, FALSE if it does not.
*/
bool ImageTextAndIconAnalyzer::isTextOrIcon(const Mat &imgBW, const cv::Rect &boundingRect, bool &isText)
{
	const int insetFromBound = 1;
	const int intensityChangeThreshText = 4;
	const int intensityChangeThreshIcon = 1;
	const int lenLim = 30;
	const int sideProp = 10;
	int intensityChangeCnt(0), x1, x2, y1, y2;

	// If it is small in both width and height, then we can assume it's a small button or icon:
	if((boundingRect.height <= lenLim) && (boundingRect.width <= lenLim)) {
		isText = false;
		return true;
	}
	
	// Search along top of bounding rectangle, if constant pixel intensity, then likely a windows form/control:
	x1 = boundingRect.x + (boundingRect.width / sideProp);
	x2 = boundingRect.x + ((sideProp - 1) * boundingRect.width / sideProp);
	y2 = y1 = (boundingRect.y + insetFromBound);
	intensityChangeCnt += scanLineForIntensityChangeCount(imgBW, x1, x2, y1, y2);
	if(isText = (intensityChangeCnt >= intensityChangeThreshText)) {
		return true;
	}

	// Search along bottom of bounding rectangle, if constant pixel intensity, then likely a windows form/control:
	y2 = y1 = (boundingRect.y + boundingRect.height - insetFromBound);
	intensityChangeCnt += scanLineForIntensityChangeCount(imgBW, x1, x2, y1, y2);
	if(isText = (intensityChangeCnt >= intensityChangeThreshText)) {
		return true;
	}

	// Search along left of bounding rectangle, if constant pixel intensity, then likely a windows form/control:
	x2 = x1 = (boundingRect.x + insetFromBound);
	y1 = (boundingRect.y + (boundingRect.height / sideProp));
	y2 = (boundingRect.y + ((sideProp - 1) * boundingRect.height / sideProp));
	intensityChangeCnt += scanLineForIntensityChangeCount(imgBW, x1, x2, y1, y2);
	if(isText = (intensityChangeCnt >= intensityChangeThreshText)) {
		return true;
	}
	

	// Search along left of bounding rectangle, if constant pixel intensity, then likely a windows form/control:
	x2 = x1 = (boundingRect.x + boundingRect.width - insetFromBound);
	intensityChangeCnt += scanLineForIntensityChangeCount(imgBW, x1, x2, y1, y2);
	if(isText = (intensityChangeCnt >= intensityChangeThreshText)) {
		return true;
	}

	return (intensityChangeCnt >= intensityChangeThreshIcon);
}


/**
--- DESCRIPTION ---
Scans a Mat object (grayscale image) for a count of pixel instensity changes along a given line. 
--- INPUTS ---
scanImg: A const reference to a Mat object that should contain the grayscale image that will be scanned.
x1: An int containing the starting x-coordinate of the scan line.
x2: An int containing the ending x-coordinate of the scan line.
y1: An int containing the starting y-cooridante of the scan line.
y2: An int containing the ending y-coordinate of the scan line.
changeLim: (OPTIONAL default=MAXINT) An int containing the max number of instensity changes found before forcing an exit from the function.
--- OUTPUT ---
An int containing the count of intensity changes along the given line.
*/
int ImageTextAndIconAnalyzer::scanLineForIntensityChangeCount(const cv::Mat &scanImg, int x1, int x2, int y1, int y2, int changeLim /*=MAXINT*/)
{
	int lastIntensity, intensity, changeCount(0);

	if(x1 == x2) { // Scanning Vertically for intesnity change:
		lastIntensity = (int)scanImg.at<uchar>(cv::Point(x1, y1));
		for(y1++ ; (y1 < y2 && changeCount < changeLim) ; y1++) {
			imgTest.at<Vec3b>(cv::Point(x1, y1))[0] = 255;
			imgTest.at<Vec3b>(cv::Point(x1, y1))[1] = 0;
			imgTest.at<Vec3b>(cv::Point(x1, y1))[2] = 0;
			if(x1 + 1 < imgTest.cols) {
				imgTest.at<Vec3b>(cv::Point(x1 + 1, y1))[0] = 255;
				imgTest.at<Vec3b>(cv::Point(x1 + 1, y1))[1] = 0;
				imgTest.at<Vec3b>(cv::Point(x1 + 1, y1))[2] = 0;
			}
			if(x1 - 1 >= 0) {
				imgTest.at<Vec3b>(cv::Point(x1 - 1, y1))[0] = 255;
				imgTest.at<Vec3b>(cv::Point(x1 - 1, y1))[1] = 0;
				imgTest.at<Vec3b>(cv::Point(x1 - 1, y1))[2] = 0;
			}
			intensity = (int)scanImg.at<uchar>(cv::Point(x1, y1));
			if(intensity != lastIntensity) { // If we can find a single point that changes in intensity, then edge is not straight line:
				changeCount++;
			}
		}
	}
	else { // Scanning Horizontally for intensity change:
		lastIntensity = (int)scanImg.at<uchar>(cv::Point(x1, y1));
		for(x1++ ; (x1 < x2 && changeCount < changeLim) ; x1++) {
			imgTest.at<Vec3b>(cv::Point(x1, y1))[0] = 0;
			imgTest.at<Vec3b>(cv::Point(x1, y1))[1] = 255;
			imgTest.at<Vec3b>(cv::Point(x1, y1))[2] = 0;
			if(y1 + 1 < imgTest.rows) {
				imgTest.at<Vec3b>(cv::Point(x1, y1 + 1))[0] = 0;
				imgTest.at<Vec3b>(cv::Point(x1, y1 + 1))[1] = 255;
				imgTest.at<Vec3b>(cv::Point(x1, y1 + 1))[2] = 0;
			}
			if(y1 - 1 >= 0) {
				imgTest.at<Vec3b>(cv::Point(x1, y1 - 1))[0] = 0;
				imgTest.at<Vec3b>(cv::Point(x1, y1 - 1))[1] = 255;
				imgTest.at<Vec3b>(cv::Point(x1, y1 - 1))[2] = 0;
			}
			intensity = (int)scanImg.at<uchar>(cv::Point(x1, y1));
			if(intensity != lastIntensity) {
				changeCount++;
			}
		}
	}

	return changeCount;
}


/**
--- DESCRIPTION ---
Filters out all found text and icons within a given image. The image must be in grayscale (uchar) format or rgb (Vec3i) format.
--- INPUTS ---
destImg: A refrence to a Mat object that will be set to the filtered image.
srcImg: A const reference to a Mat object containing the image to be filtered.
textAndIconBoundingRects: A const reference to a vector of rect objects containing all of the bounding rectangles of the text and icons
						  to be filtered.
*/
void ImageTextAndIconAnalyzer::filterOutTextOrIcons(cv::Mat &destImg, const cv::Mat &srcImg, const std::vector<cv::Rect> *textBoundingRects /*=nullptr*/,
												    const std::vector<cv::Rect> *iconBoundingRects /*=nullptr*/)
{
	int x, y;
	Scalar upperRightIntensity;
	const std::vector<cv::Rect> *textOrIconBoundingRects;

	destImg = srcImg.clone();
	textOrIconBoundingRects = textBoundingRects;
	for(int i = 0 ; i < 2 ; i++) {
		if(textOrIconBoundingRects != nullptr) {
            for(uint i = 0 ; i < textOrIconBoundingRects->size() ; i++) {
				const cv::Rect &rect = textOrIconBoundingRects->at(i);
				x = (rect.x + rect.width - 1);
				y = (rect.y + 1);
				if(srcImg.channels() == 3) { // For color image:
					upperRightIntensity = Scalar(destImg.at<cv::Vec3b>(cv::Point(x, y)));
				}
				else { // For grayscale image:
					upperRightIntensity = Scalar(destImg.at<uchar>(cv::Point(x, y)));
				}
				cv::rectangle(destImg, rect, upperRightIntensity, CV_FILLED);
			}
		}
		textOrIconBoundingRects = iconBoundingRects;
	}
}


/*void detectText(const Mat &src)
{
    // Extract channels to be processed individually
    vector<Mat> channels;
    computeNMChannels(src, channels);

    int cn = (int)channels.size();
    // Append negative channels to detect ER- (bright regions over dark background)
    for (int c = 0; c < cn-1; c++)
        channels.push_back(255-channels[c]);

    // Create ERFilter objects with the 1st and 2nd stage default classifiers
    Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"),16,0.00015f,0.13f,0.2f,true,0.1f);
    Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"),0.5);

    vector<vector<ERStat> > regions(channels.size());
    // Apply the default cascade classifier to each independent channel (could be done in parallel)
    for (int c=0; c<(int)channels.size(); c++)
    {
        er_filter1->run(channels[c], regions[c]);
        er_filter2->run(channels[c], regions[c]);
    }

    // Detect character groups
    vector< vector<Vec2i> > region_groups;
    vector<Rect> groups_boxes;
    erGrouping(src, channels, regions, region_groups, groups_boxes, ERGROUPING_ORIENTATION_HORIZ);
    //erGrouping(src, channels, regions, region_groups, groups_boxes, ERGROUPING_ORIENTATION_ANY, "./trained_classifier_erGrouping.xml", 0.5);

    // draw groups
    groups_draw(src, groups_boxes);
    imshow("grouping",src);

    if(waitKey(0) == 101)
        er_show(channels,regions);

    // memory clean-up
    er_filter1.release();
    er_filter2.release();
    regions.clear();
    if (!groups_boxes.empty()) {
        groups_boxes.clear();
    }
}*/


// Private do nothing constructor and assignment op for static class!
ImageTextAndIconAnalyzer::ImageTextAndIconAnalyzer(){}
ImageTextAndIconAnalyzer& ImageTextAndIconAnalyzer::operator=(const ImageTextAndIconAnalyzer &rhs){ return *this; }
