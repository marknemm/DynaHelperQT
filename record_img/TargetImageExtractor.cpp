#include "TargetImageExtractor.h"
#include "ImageConverter.h"
#include "ImageTextAndIconAnalyzer.h"
#include "NativeImageUtil.h"
#include "BoundsChecker.h"
#include "RecordImgDbg.h"
#include "model/MacroEvent.h"
#include <math.h>
#include <opencv2/features2d/features2d.hpp>
#include <QDebug>


/**
--- DESCRIPTION ---
Extracts the target image and determines its type using an iteration technique. The extracion is based in differentiating between the background and
foreground difference in contrast. This iterative version will start at an area local to the target, and will expand outward at a given expansion rate
until the technique is executed on the entire source (screenshot) image. This will take longer, but will be more effective because it will take the
contrast in smaller areas at first, and will be able to focus on a local foreground and background.
--- INPUTS ---
destImage: The narrowed down image. The image will be in grayscale regardless of the type of srcImage.
targetImgType: Will be set to the determined type of the target image.
srcImage: A reference to a Mat object representing the image (screenshot) that will be narrowed down to the target object of an associated action.
loc: A const reference to the location of the mouse cursor in screen pixel units.
cursorType: The type of the cursor at the time when the action over the target image was initially recorded.
expansionRate: (OPTIONAL default=2) The rate at which the base width and height values will expand outward.
baseW: (OPTIONAL default=10) The base width of the region that will be analyzed for extraction.
baseH: (OPTIONAL default=10) The base height of the region that will be analyzed for extraction.
*/
cv::Rect TargetImageExtractor::iterativeExtractBasedInConstrast(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType,
																int expansionRate /*=2*/, int baseW /*=10*/, int baseH /*=10*/)
{
	const int repeatLim(2);
	Mat tempDestImg, imgGray;
	cv::Rect srcImgRect, tempExtractRect, largestSeenExtractRect, lastExtractRect, roi;
	bool iterationFinished(false);
	srcImgRect = cv::Rect(0, 0, srcImg.cols, srcImg.rows);
	cv::Point locInRoi, offsetXY;
	int lastExtractRectRepeatCnt(0), largestExtractRectRepeatCnt(0), largestExtractRectUnseenCnt(0);

    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);
	targetImgType = TARGET_IMG_UNDEF;
	while(!iterationFinished) {
		// Check for repeat.
		if(BoundsChecker::areRectsEqual(lastExtractRect, tempExtractRect)) {
			lastExtractRectRepeatCnt++;
		}
		else { lastExtractRectRepeatCnt = 0; }

		// Check for new largest seen bounds of extract region.
		if(!BoundsChecker::isRectSmaller(tempExtractRect, largestSeenExtractRect)
			&& !BoundsChecker::areRectsEqual(tempExtractRect, largestSeenExtractRect))
		{
			largestSeenExtractRect = tempExtractRect;
			largestExtractRectRepeatCnt = 0;
		}

		// Check for repeat in largest seen rectangle.
		if(BoundsChecker::areRectsEqual(largestSeenExtractRect, tempExtractRect)) {
			largestExtractRectRepeatCnt++;
			largestExtractRectUnseenCnt = 0;
		}
		else if(++largestExtractRectUnseenCnt == 5) {
			largestSeenExtractRect = tempExtractRect; // Reset so no infinite loop!
		}

		// Update parameters for this iteration and see if finished.
		lastExtractRect = tempExtractRect;
		iterationFinished = getNextIterationParams(srcImgRect, loc, locInRoi, roi, tempExtractRect, targetImgType, expansionRate, baseW, baseH);
		iterationFinished = (iterationFinished || (lastExtractRectRepeatCnt == repeatLim) || (largestExtractRectRepeatCnt == repeatLim));
		if(iterationFinished) { break; } // STOP!!!

		// Perform extraction for this iteration.
		tempExtractRect = extractBasedInContrast(tempDestImg, targetImgType, imgGray(roi), locInRoi, cursorType);
		tempExtractRect.x += roi.x;
		tempExtractRect.y += roi.y;
		RecordImgDbg::displayImg(tempDestImg, "Temp Dest Img");
	}

	destImg = tempDestImg;
	return tempExtractRect;
}


/**
--- DESCRIPTION ---
Extracts the target image and determines its type. The extracion is based in differentiating between the background and foreground
difference in contrast.
--- INPUTS ---
destImage: The narrowed down image. The image will be in grayscale regardless of the type of srcImage.
targetImgType: Will be set to the determined type of the target image.
srcImage: A reference to a Mat object representing the image (screenshot) that will be narrowed down to the target object of an associated action.
loc: A const reference to the location of the mouse cursor in screen pixel units.
cursorType: The type of the cursor at the time when the action over the target image was initially recorded.
*/
cv::Rect TargetImageExtractor::extractBasedInContrast(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Rect> textBoundingRects, iconBoundingRects, boundsRects;
    //std::vector<Vec4i> hierarchy;
	Mat textBoundImg, boundImg, imgGray, noTextAndIcons;

	//getSmallestBoundRectAtLoc(srcImg, loc);

    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);
	cv::Rect textAndIconRect = ImageTextAndIconAnalyzer::extractTextAndIconsAtLoc(textBoundImg, imgGray, loc, textBoundingRects, iconBoundingRects);
	ImageTextAndIconAnalyzer::filterOutTextOrIcons(noTextAndIcons, imgGray, &textBoundingRects, &iconBoundingRects);
	cv::Rect boundRect = extractBoundsAtLocNoText(boundImg, noTextAndIcons, loc, boundsRects);
	cv::Rect targetRect = determineTarget(destImg, targetImgType, textBoundImg, textAndIconRect, boundImg, boundRect, imgGray, cursorType, loc);
	return targetRect;
}


/**
--- DESCRIPTION ---
Extracts the target image and determines its type. The extracion is based in differentiating between the background and foreground
from difference in contrast.
--- INPUTS ---
srcImage: A reference to a Mat object representing the image that will be narrowed down to the target object of an associated action.
targetImgType: Will be set to the determined type of the target image.
destImage: The narrowed down image. The image will be in grayscale regardless of the type of srcImage.
loc: A const reference to the location of the mouse cursor in screen pixel units.
cursorType: The type of the cursor at the time when the action over the target image was initially recorded.
*/
cv::Rect TargetImageExtractor::extractBasedInMser(Mat &destImg, int &targetImgType, const Mat &srcImg, const cv::Point &loc, int cursorType)
{
	// TODO doesn't work well (maybe different parameters will yield better results...)!!!
    MSER ms(5, 0, 14400, 1, 0.2, 200, 1.01, 0.003, 25);
    vector<vector<cv::Point>> regions;
	Mat imgGray;

    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);
    ms(imgGray, regions);
    destImg = ImageConverter::convertGrayToRGB(imgGray);
    for(uint i = 0 ; i < regions.size() ; i++) {
        rectangle(destImg, boundingRect(regions[i]), Scalar(255));
    }

	RecordImgDbg::displayImg(destImg);
	return cv::Rect();
}


cv::Rect TargetImageExtractor::getSmallestBoundRectAtLoc(const Mat &srcImg, const cv::Point &loc)
{
	cv::Rect smallestBoundRect(-1, -1, 10000, 10000), tempRect;
	Mat imgGray, imgBWEdges, testImg, morphKernel;
	std::vector<std::vector<cv::Point>> contours;
    std::vector<Vec4i> hierarchy;
	std::vector<cv::Point> approx;
	int vtc;

	srcImg.copyTo(testImg);
    imgGray = ImageConverter::convertToGrayIfOtherFormat(srcImg);

	Canny(imgGray, imgBWEdges, 0, 50, 5);
	RecordImgDbg::displayImg(imgBWEdges);
	findContours(imgBWEdges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for(uint i = 0 ; i < contours.size() ; i++) {
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), false) * 0.02, true);
		vtc = approx.size();

		// Skip non-convex and non-4-point polygons.
		if(!isContourConvex(approx) || (vtc != 4)) { continue; }

        tempRect = boundingRect(contours[i]);
		rectangle(testImg, tempRect, Scalar(255, 0, 0), 4);
		if(BoundsChecker::isLocInRect(loc, tempRect) && BoundsChecker::isRectSmaller(tempRect, smallestBoundRect)) {
			smallestBoundRect = tempRect;
		}
	}

	RecordImgDbg::displayImg(testImg);
	return smallestBoundRect;
}


bool TargetImageExtractor::getNextIterationParams(const cv::Rect &srcImgRect, const cv::Point &globalLoc, cv::Point &roiLoc, cv::Rect &roi,
												  const cv::Rect &extractRect, int targetImgType, int expansionRate, int baseW, int baseH)
{
	static bool fixX(false), fixY(false), fixX2(false), fixY2(false);
	int width(baseW), height(baseH);
	bool iterationFinished(false);
	cv::Point offsetXY;
	cv::Rect oldRoi(roi);

	if(targetImgType != TARGET_IMG_UNDEF) {
		width = (extractRect.width * expansionRate);
		height = (extractRect.height * expansionRate);
	}
	else {
		fixX = fixY = fixX2 = fixY2 = false;
	}

	// Set new roi bounds and action loc in roi
	roi = cv::Rect(globalLoc.x - (width / 2), globalLoc.y - (height / 2), width, height); // center roi around global loc.
	roiLoc = cv::Point(roi.width / 2, roi.height / 2);
	if(targetImgType != TARGET_IMG_TEXT) {
		checkForFixedRoiBounds(fixX, fixY, fixX2, fixY2, oldRoi, extractRect);
		setRoiBoundsToFitFixed(fixX, fixY, fixX2, fixY2, oldRoi, roi, extractRect, roiLoc);
	}

	iterationFinished = (width >= srcImgRect.width && height >= srcImgRect.height);
	iterationFinished = (iterationFinished || (fixX && fixY && fixX2 && fixY2));
	BoundsChecker::makeSureRectIsInScreenBounds(roi, srcImgRect, &offsetXY); // Make sure our roi is within screenshot!
	// If either X or Y are moved to zero in the ROI because they fall below 0, then reset locInRoi to global value!
	if(offsetXY.x != 0) { roiLoc.x = globalLoc.x; }
	if(offsetXY.y != 0) { roiLoc.y = globalLoc.y; }

	return iterationFinished;
}


void TargetImageExtractor::checkForFixedRoiBounds(bool &fixX, bool &fixY, bool &fixX2, bool &fixY2, const cv::Rect &roi, const cv::Rect &extractRect)
{
	if(roi.x != extractRect.x) { fixX = true; }
	if(roi.y != extractRect.y) { fixY = true; }
	if(roi.x + roi.width != extractRect.x + extractRect.width) { fixX2 = true; }
	if(roi.y + roi.height != extractRect.y + extractRect.height) { fixY2 = true; }
}


void TargetImageExtractor::setRoiBoundsToFitFixed(bool fixX, bool fixY, bool fixX2, bool fixY2, const cv::Rect &oldRoi,
												  cv::Rect &roi, const cv::Rect &extractRect, cv::Point &roiLoc)
{
	int diffX, diffY, diffX2, diffY2;
	if(fixX) {
		diffX = (extractRect.x - roi.x);
		roi.width -= diffX;
		roiLoc.x -= diffX;
		roi.x = extractRect.x;
	}
	if(fixY) {
		diffY = (extractRect.y - roi.y);
		roi.height -= diffY;
		roiLoc.y -= diffY;
		roi.y = extractRect.y;
	}
	if(fixX2) {
		diffX2 = ((roi.x + roi.width) - (extractRect.x + extractRect.width));
		roi.width -= diffX2;
	}
	if(fixY2) {
		diffY2 = ((roi.y + roi.height) - (extractRect.y + extractRect.height));
		roi.height -= diffY2;
	}
}


/**
--- DESCRIPTION ---
Finds all bounding structures in the (grayscale) screenshot image related to the triggering action. Then, it extracts any found bounds
local to (surrounding) the triggering action.
--- INPUTS ---
destImg: A reference to a Mat object that will be set to the bounding structure image local to the triggering action.
srcImg: A const reference to a Mat object that should contain the screenshot image related to the triggering action (in grayscale).
loc: A const reference to a Point object that should contain the screen coordinates of the triggering action.
boundsRects: A reference to a vector of rect objects that will be filled with all of the bounding rectangles of found bounding structures.
--- OUTPUT ---
A rectangle object that contains the bounds around the extracted bounding structure image local to the triggering action.
*/
cv::Rect TargetImageExtractor::extractBoundsAtLocNoText(Mat &destImg, const Mat &srcImg, const cv::Point &loc, std::vector<cv::Rect> &boundsRects)
{
	Mat grayImg;
	cv::Rect tightestBoundRect, srcImgBounds;
	const int margin = 2;

    grayImg = ImageConverter::convertToGrayIfOtherFormat(srcImg);
	// Simply flood fill (paint bucket) without changing color and get bounding rectangle:
	floodFill(grayImg, loc, Scalar(grayImg.at<uchar>(loc)), &tightestBoundRect, Scalar(0), Scalar(0));

	// Apply margin to our tightest bound rect.
	tightestBoundRect.x -= margin;
	tightestBoundRect.y -= margin;
	tightestBoundRect.width += (margin * 2);
	tightestBoundRect.height += (margin * 2);

	// Make sure our tightest bound rect for the bound with no text (windows form bound) is inside srcImg bounds!
	srcImgBounds = cv::Rect(0, 0, srcImg.cols, srcImg.rows);
	BoundsChecker::makeSureRectIsInScreenBounds(tightestBoundRect, srcImgBounds);

	destImg = srcImg(cv::Range(tightestBoundRect.y, tightestBoundRect.y + tightestBoundRect.height),
					 cv::Range(tightestBoundRect.x, tightestBoundRect.x + tightestBoundRect.width));
	return tightestBoundRect;
}


/**
--- DESCRIPTION ---
Determines the final target image, its type, and its bounds based off of data associated with the triggering action.
--- INPUTS ---
destImg: A reference to a Mat object that will be set to the target image (in grayscale).
targetImageType: A reference to an int that will be set to the target image's type.
textImg: A const reference to a Mat object that should cotain the text or icon found within the possible target area.
textRect: A const reference to the bounding rectangle of the textImg in relation to the entire screen.
boundImg: A const reference to a Mat object that should contain the bounding region of the possible target area.
boundRect: A const reference to the bounding rectangle of the boundImg in relation to the entire screen.
grayImg: A const reference to a Mat object that should contain the entire screenshot triggered by an action (in grayscale).
cursorType: An int containing the type of cursor associated with the action that triggered this function.
--- OUTPUT ---
A rectangle containing the bounds for the final target image in relation to the entire screen.
*/
cv::Rect TargetImageExtractor::determineTarget(cv::Mat &destImg, int &targetImageType, const cv::Mat &textImg, const cv::Rect &textRect,
											   const cv::Mat &boundImg, cv::Rect &boundRect, const cv::Mat &grayImg, int cursorType, const cv::Point &loc)
{
    //const int deltaBound(25);
	bool left, top, right, bottom;

	// Image debug display code.
	Mat imgClr;
    imgClr = ImageConverter::convertGrayToRGB(grayImg);
	rectangle(imgClr, textRect, Scalar(0, 0, 255), 2);
	rectangle(imgClr, boundRect, Scalar(0, 255, 0), 2);
	circle(imgClr, loc, 3, Scalar(255, 0 , 0), CV_FILLED);
	RecordImgDbg::displayImg(imgClr, "Text & Rect Bounds");

	// If the cursor type of the action is an I-Beam:
	if(cursorType == CURSOR_IBEAM) {
		destImg = boundImg; // Filtered text with this!
		//destImg = grayImg(boundRect);
		targetImageType = TARGET_IMG_FILTER_TEXT;
		return boundRect;
	}

	// If no text image was extracted OR the extracted outer bound of a form is tight enough to the text bound:
	if(textImg.rows == 0 || getSidesBoundTightEnough(textRect, boundRect, left, top, right, bottom) == 4) {
		destImg = grayImg(boundRect);
		targetImageType = TARGET_IMG_CONTROL;
		return boundRect;
	}

	// Deduced that the text itself appears to be the target:
	destImg = textImg;
	targetImageType = TARGET_IMG_TEXT;
	return textRect;
}



/**
--- DESCRIPTION ---
Gets the number of sides of the bounding text rectangle that are close enough to the bounding windows form rectangle.
--- INPUTS ---
textRect: The bounding text rectangle.
boundRect: The bounding windows form rectangle.
--- OUTPUT ---
The number of sides bound tightly enough.
*/
int TargetImageExtractor::getSidesBoundTightEnough(const cv::Rect &textRect, const cv::Rect &boundRect,
												   bool &left, bool &top, bool &right, bool &bottom)
{
	int deltaX, deltaY, deltaX2, deltaY2, deltaBoundX, deltaBoundY, numberOfSidesBoundTightEnough(0);
	
	deltaX = (textRect.x - boundRect.x);
	deltaY = (textRect.y - boundRect.y);
	deltaX2 = ((boundRect.x + boundRect.width) - (textRect.x + textRect.width));
	deltaY2 = ((boundRect.y + boundRect.height) - (textRect.y + textRect.height));
	deltaBoundY = (textRect.height * 1.5);
	deltaBoundX = (textRect.width / 2);
	
	if(left = (deltaX <= deltaBoundX)) { numberOfSidesBoundTightEnough++; }
	if(top = (deltaY <= deltaBoundY)) { numberOfSidesBoundTightEnough++; }
	if(right = (deltaX2 <= deltaBoundX)) { numberOfSidesBoundTightEnough++; }
	if(bottom = (deltaY2 <= deltaBoundY)) { numberOfSidesBoundTightEnough++; }

	return numberOfSidesBoundTightEnough;
}


/**
--- DESCRIPTION ---
Private default constructor and assignment operator for a RecordImageUtil Object. Should never be instantiated (all methods are static).
*/
TargetImageExtractor::TargetImageExtractor() {}
TargetImageExtractor& TargetImageExtractor::operator=(const TargetImageExtractor &rhs) { return *this; }
