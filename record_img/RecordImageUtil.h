#ifndef RECORD_IMAGE_UTIL_H
#define RECORD_IMAGE_UTIL_H


#include "RecordImgDbg.h"
#include "model/MacroEvent.h"
#include <cv.h>
#include <QRect>
#include <QPoint>
#include <QColor>
using namespace cv;


/**
--- DESCRIPTION ---
Toolkit for utility functions associated with Record Image Recording and Analyzation/Searching.
--- AUTHOR ---
Mark Nemmer
*/
class RecordImageUtil {

public:

    /**
     * @brief takeScreenshotAndSave
     * Takes a screenshot.
     * @return
     * The screenshot that was taken.
     */
    static QImage takeScreenshot();

    /**
     * @brief isolateTargetImg
     * Isolates a sub-portion of a given image known as the target region of interest (target ROI).
     * This ROI signifies the isolated target of a location sensitive mouse event. It is later used
     * to search for the target of the mouse event when it is replayed if the target has moved from
     * the expected location. Also determines other useful properties related to the target.
     * @param targetImgType (OUTPUT)
     * The type of the target. Used as extra information in locating the target of an event.
     * @param actionCursorType (OUTPUT)
     * The type of cursor used when the event took place. Gives extra information about the target and
     * helps ensure that the cursor is over a correct target when replaying the event.
     * @param screenshot
     * The screenshot image to find the target ROI within.
     * NOTE: May modify internally by swapping image color channels when converting. Make a copy if caller cares!!!
     * @param actionLoc
     * The location of the mouse event.
     * @return
     * The isolated region of interest (ROI) for the target of a given location sensitive mouse event.
     */
    static QRect isolateTargetImg(TargetImgType &targetImgType,
                                  CursorType &actionCursorType,
                                  QImage &screenshot,
                                  const QPoint &actionLoc);

    /**
     * @brief findTargetImg
     * Finds the target image in a given screenshot associated with a location sensitive mouse event.
     * @param screenshotFileName
     * The full path and name of the sceenshot file. Used to search for the target in.
     * @param targetROI
     * The isolated region of interest (ROI) for the target of a given location sensitive mouse event.
     * This is where we expect the event target to be contained within.
     * @return
     * The new targetROI. It should be equivalent to the input targetROI if the target has not moved.
     */
    static QRect findTargetImg(const QString &screenshotPath,
                               const QRect &targetROI);


    /**
     * @brief cropImg
     * Crops a given image and optionally draws an event location marker on the cropped image.
     * @param imgToCrop
     * The image to crop.
     * @param cropRect
     * The crop rectangle. If the x or y origin coordinates are negative, then the crop rectangle is
     * evenly positioned around the eventLoc. Will be set to the new bounds for the cropped image.
     * @param fillRemainClr (OPTIONAL)
     * Set to a color if when imgToCrop is smaller than the size of cropRect, then the resulting crop image
     * should be filled with the color in the difference of boundary between imgToCrop and cropRect.
     * @param eventLoc (OPTIONAL)
     * The location of the event in imgToCrop coordinates if eventLocOff is not given.
     * The location of the event in global screen coordinates if eventLocOff is given.
     * @param eventLocOff (OPTIONAL)
     * The x and y offset to apply to global screen coordinates given for eventLoc so that we can convert to imgToCrop coordinates.
     * This should be the x,y origin of imgToCrop in screen coordinates.
     * @return
     * The resulting crop image.
     */
    static QImage cropImg(const QImage &imgToCrop,
                          QRect &cropRect,
                          const Qt::GlobalColor *fillRemainClr = nullptr,
                          const QPoint *eventLoc = nullptr,
                          const QPoint *eventLocOff = nullptr);

    /**
     * @brief markEventLoc
     * Marks an input image with an event location dot of a specified color.
     * @param imgToMark
     * The input image to mark.
     * @param eventLoc
     * The location of the event. If eventLocOff is not given, then this is assumed to be in imgToMark coordinates.
     * If eventLocOff is given, then this should be in global screen coordinates.
     * @param eventLocOff (OPTIONAL)
     * The offset of eventLoc. If given, then eventLoc will be offset by this amount. Should be set to the X,Y origin
     * of imgToMark in global screen coordinates if eventLoc is in globla screen coordinates.
     * @param markClr (OPTIONAL)
     * The color to draw the marker dot with. The default is red.
     * @return
     * The marked copy of imgToMark.
     */
    static QImage markEventLoc(const QImage &imgToMark,
                               const QPoint &eventLoc,
                               const QPoint *eventLocOff = nullptr,
                               const QColor &markClr = QColor(255, 0, 0));

private:


    // Private default constructor and assignment operator for a RecordImageUtil Object. Should never be instantiated (all methods are static).
    RecordImageUtil();
	RecordImageUtil& operator=(const RecordImageUtil &rhs);
};


#endif
