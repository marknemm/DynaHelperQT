#include "BoundsChecker.h"
#include <QDebug>


/**
--- DESCRIPTION ---
Determines if a Point falls within a given Rectangle.
--- INPUTS ---
loc: A const reference to the Point containing the location that will be checked for its locality within a given rectangle.
rect: A const reference to a Rect object containing the rectangle that will be checked to see if a given loc falls within its bounds.
--- OUTPUT ---
A bool containing TRUE if loc is within rect, FALSE otherwise.
*/
bool BoundsChecker::isLocInRect(const cv::Point &loc, const cv::Rect &rect)
{
	return (loc.x >= rect.x && loc.x <= (rect.x + rect.width) && loc.y >= rect.y && loc.y <= (rect.y + rect.height));
}


/**
--- DESCRIPTION ---
Compares the sizes (areas) of two rectangles and determines if the one to be checked is smaller than the one that is being compared to.
--- INPUTS ---
checkRect: A const reference to the rectangle object to check whether it is smaller than a comparison rectangle.
compareRect: A const reference to the rectangle object to compare the size of checkRect to.
--- OUTPUT ---
A bool containing TRUE if the checkREct is smaller (area-wise) than compareRect, FALSE if not.
*/
bool BoundsChecker::isRectSmaller(const cv::Rect &checkRect, const cv::Rect &compareRect)
{
	int areaCheck = (checkRect.width * checkRect.height);
	int areaCompare = (compareRect.width * compareRect.height);
	return (areaCheck < areaCompare);
}


/**
--- DESCRIPTION ---
Makes sure that a given rectangle is within the bounds of the screen rectangle. If it is not within the bounds, then it is fitted to the screen rectangle.
--- INPUTS ---
checkRect: The rectangle that will be checked, and if necessary, fitted to the screen rectangle.
screenRect: The screen rectangle.
*/
void BoundsChecker::makeSureRectIsInScreenBounds(cv::Rect &checkRect, const cv::Rect &screenRect,
												 cv::Point *offsetXY /*=nullptr*/, cv::Size *offsetWH/*=nullptr*/)
{
	int checkRectX2, checkRectY2, offsetW, offsetH;
	// Handle X.
	if(checkRect.x < 0) {
		if(offsetXY != nullptr) { offsetXY->x = (0 - checkRect.x); }
		checkRect.x = 0;
	}
	// Handle Y.
	if(checkRect.y < 0) {
		if(offsetXY != nullptr) { offsetXY->y = (0 - checkRect.y); }
		checkRect.y = 0;
	}
	// Handle W.
	checkRectX2 = (checkRect.x + checkRect.width);
	if(checkRectX2 > screenRect.width) {
		offsetW = (checkRectX2 - screenRect.width);
		if(offsetWH != nullptr) { offsetWH->width = offsetW; }
		checkRect.width -= offsetW;
	}
	// Handle H.
	checkRectY2 = (checkRect.y + checkRect.height);
	if(checkRectY2 > screenRect.height) {
		offsetH = (checkRectY2 - screenRect.height);
		if(offsetWH != nullptr) { offsetWH->height = offsetH; }
		checkRect.height -= offsetH;
	}
}


/**
--- DESCRIPTION ---
Compares two rectangles, and determines if their coordinates and sizes are both equal.
--- INPUTS ---
rect1: The first rectangle.
rect2: The second rectangle.
--- OUTPUT ---
True if they are equal in position and size, false otherwise.
*/
bool BoundsChecker::areRectsEqual(const cv::Rect &rect1, const cv::Rect &rect2)
{
	return (rect1.x == rect2.x && rect1.y == rect2.y && rect1.width == rect2.width && rect1.height == rect2.height);
}


/**
--- DESCRIPTION ---
Gets the smallest bounding rectangle around a given location. The location must be in a non-negative space.
--- INPUTS ---
rects: The rectangles to check.
loc: The location.
--- OUTPUT ---
The smallest bounding rectangle if one was found. If one was not found, then a rectangle with X=-1, Y=-1, W=10000, H=10000 is returned.
*/
cv::Rect BoundsChecker::getSmallestBoundingRect(const std::vector<cv::Rect> &rects, const cv::Point &loc)
{
	const int randomHigh = 10000;
	const cv::Rect *tempRect;
    cv::Rect tempTightestBoundRect(-1, -1, randomHigh, randomHigh);
    const cv::Rect *tightestBoundRect = &tempTightestBoundRect;
	for(int i = 0 ; i < rects.size() ; i++) {
		tempRect = &rects.at(i);
		if(BoundsChecker::isLocInRect(loc, *tempRect) && BoundsChecker::isRectSmaller(*tempRect, *tightestBoundRect)) {
			tightestBoundRect = tempRect;
		}
	}
	return *tightestBoundRect;
}


QPoint BoundsChecker::offsetPoint(const QPoint &ptToOffset,
                                  const QPoint &offsetXY,
                                  bool subOffset)
{
    QPoint offsetPt;
    // Subtract the offset X and Y.
    if (subOffset) {
        offsetPt.setX(ptToOffset.x() - offsetXY.x());
        offsetPt.setY(ptToOffset.y() - offsetXY.y());
    }
    // Add the offset X and Y.
    else {
        offsetPt.setX(ptToOffset.x() + offsetXY.x());
        offsetPt.setY(ptToOffset.y() + offsetXY.y());
    }
    return offsetPt;
}


QRect BoundsChecker::refineCropBounds(const QRect &rawCropRect,
                                      const QSize *cropImgSize,
                                      const QPoint *centerPt)
{
    QRect cropRect = rawCropRect;

    // Must we center the crop rectangle around center point?
    if (centerPt != nullptr) {
        // Calculate cropRect X, Y position.
        cropRect.moveLeft(centerPt->x() - (rawCropRect.width() / 2));
        cropRect.moveTop(centerPt->y() - (rawCropRect.height() / 2));
    }

    // Are we croping beyond the left X boundary or top Y boundary of our imgToCrop?
    if (cropRect.x() < 0) cropRect.moveLeft(0);
    if (cropRect.y() < 0) cropRect.moveTop(0);

    // Do we have to fit our crop rectangle inside some crop image bounds?
    if (cropImgSize != nullptr) {
        // Are we croping beyond the right X boundary of our imgToCrop?
        if (cropRect.right() > cropImgSize->width()) {
            cropRect.moveLeft(cropRect.x() - (cropRect.right() - cropImgSize->width())); // Move left by overflow amount on right.
            // We definitely cannot fit our crop rectange inside the crop image, so just make it width of crop image.
            if (cropRect.x() < 0) {
                cropRect.moveLeft(0);
                cropRect.setWidth(cropImgSize->width());
            }
        }

        // Are we croping beyond the right X boundary of our imgToCrop?
        if (cropRect.bottom() > cropImgSize->height()) {
            cropRect.moveTop(cropRect.y() - (cropRect.bottom() - cropImgSize->height())); // Move up by overflow amount on bottom.
            // We definitely cannot fit our crop rectange inside the crop image, so just make it height of crop image.
            if (cropRect.y() < 0) {
                cropRect.moveTop(0);
                cropRect.setHeight(cropImgSize->height());
            }
        }
    }

    return cropRect;
}
