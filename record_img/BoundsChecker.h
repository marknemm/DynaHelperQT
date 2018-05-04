#ifndef BOUNDS_CHECKER_H
#define BOUNDS_CHECKER_H


#include <cv.h>
#include <QPoint>
#include <QRect>
#include <QSize>


class BoundsChecker {

public:

    static bool isLocInRect(const cv::Point &loc,
                            const cv::Rect &rect);

    static bool isRectSmaller(const cv::Rect &checkRect,
                              const cv::Rect &compareRect);

    static void makeSureRectIsInScreenBounds(cv::Rect &checkRect,
                                             const cv::Rect &screenRect,
                                             cv::Point *offsetXY=nullptr,
                                             cv::Size *offsetWH=nullptr);

    static bool areRectsEqual(const cv::Rect &rect1,
                              const cv::Rect &rect2);

    static cv::Rect getSmallestBoundingRect(const std::vector<cv::Rect> &rects,
                                            const cv::Point &loc);

    /**
     * @brief offsetPoint
     * Offsets a given ptToOffset by an X and Y amount in offsetXY.
     * @param ptToOffset
     * The point to offset.
     * @param offsetXY
     * The X and Y offset amounts in point form.
     * @param subOffset (OPTIONAL)
     * Set to true if the X and Y offset should be subtracted instead of added. Default is false for add offset.
     * @return
     * The offset point.
     */
    static QPoint offsetPoint(const QPoint &ptToOffset,
                              const QPoint &offsetXY,
                              bool subOffset = false);

    /**
     * @brief refineCropBounds
     * Refines a crop boundary rectangle for a given crop image.
     * @param rawCropRect
     * The raw crop rectangle that will be refined.
     * @param cropImgSize (OPTIONAL)
     * The size of the crop image. If given, the crop rectangle will be limited to the boundary of the crop image.
     * @param centerPt (OPTIONAL)
     * The center point of the crop rectangle. If given, rawCropRect will be centered around this point.
     * If raw crop rect will have an X,Y origin below 0, then it will be adjusted to 0 while maintaining its size.
     * If raw crop rect will go beyond the right and/or bottom boundaries of the crop image (cropImgSize is also supplied),
     * then the crop rect will be adjusted to be moved left and/or up so that it remains within the bounds of crop image.
     * @return
     */
    static QRect refineCropBounds(const QRect &rawCropRect,
                                  const QSize *cropImgSize = nullptr,
                                  const QPoint *centerPt = nullptr);

private:

    // Private default constructor and assignment operator. Should never be instantiated (all methods are static).
    BoundsChecker();
    BoundsChecker& operator=(const BoundsChecker &rhs);
};


#endif
