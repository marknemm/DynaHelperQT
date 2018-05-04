#include "RecordImageUtil.h"
#include "ImageSearcher.h"
#include "ImageConverter.h"
#include "NativeImageUtil.h"
#include "TargetImageExtractor.h"
#include "BoundsChecker.h"
#include <QThread>
#include <exception>
#include <QPainter>
#include <cmath>
#include <QDebug>


QImage RecordImageUtil::takeScreenshot()
{
    return NativeImageUtil::takeScreenshot();
}


QRect RecordImageUtil::isolateTargetImg(TargetImgType &targetImgType,
                                        CursorType &actionCursorType,
                                        QImage &screenshot,
                                        const QPoint &actionLoc)
{
    QRect qTargetRect;
    cv::Rect screenRect,
             targetRect;
    cv::Point cvActionLoc;
    Mat cvScreenshot,
        snapshot;

    NativeImageUtil::copyQPointToCvPoint(cvActionLoc, actionLoc);
    cvScreenshot = ImageConverter::convertQImageToMat(screenshot);
	NativeImageUtil::getScreenBound(screenRect);
    resize(cvScreenshot, cvScreenshot, screenRect.size());
    targetRect = TargetImageExtractor::extractBasedInContrast(snapshot, (int&)targetImgType, cvScreenshot, cvActionLoc, actionCursorType);

    if (targetRect.width > 300 && targetRect.height > 300) {
        targetRect = TargetImageExtractor::iterativeExtractBasedInConstrast(snapshot, (int&)targetImgType, cvScreenshot,
                                                                            cvActionLoc, actionCursorType);
    }

    NativeImageUtil::copyCvRectToQRECT(qTargetRect, targetRect);
    screenshot = screenshot.copy(qTargetRect);

    return qTargetRect;
}

QRect RecordImageUtil::findTargetImg(const QString &screenshotPath,
                                     const QRect &targetROI)
{
    cv::Point targetImgOrgUpdt,
              targetImgOrg;
    cv::Rect screenRect,
             cvTargetROI;

    NativeImageUtil::copyQRECTtoCvRect(cvTargetROI, targetROI);
    targetImgOrg = cvTargetROI.tl();

    QThread::msleep(100);
    QImage qScreenshot = NativeImageUtil::takeScreenshot();

    QImage qTargetImg(screenshotPath);
    cv::Mat targetImg = ImageConverter::convertQImageToMat(qTargetImg);
    targetImg = targetImg(cvTargetROI);

    Mat screenshot = ImageConverter::convertQImageToMat(qScreenshot);
	NativeImageUtil::getScreenBound(screenRect);
	resize(screenshot, screenshot, screenRect.size());
    screenshot = ImageConverter::convertRGBToGray(screenshot);
	targetImgOrgUpdt = ImageSearcher::templateMatchSearch(targetImg, screenshot, targetImgOrg, CV_TM_SQDIFF);
	/*targetImgOrgUpdt = ImageSearcher::featureBasedSearch(targetImg, screenshot, targetImgOrg);
	if(targetImgOrgUpdt.x < 0 ) { // If NO features could be extracted from a very basic/small image.
		targetImgOrgUpdt = ImageSearcher::templateMatchSearch(targetImg, screenshot, targetImgOrg, CV_TM_SQDIFF);
	}*/

    QPoint qTargetImgOrgUpdt;
    NativeImageUtil::copyCvPointToQPoint(qTargetImgOrgUpdt, targetImgOrgUpdt);
    return QRect(qTargetImgOrgUpdt, targetROI.size());
}


QImage RecordImageUtil::cropImg(const QImage &imgToCrop,
                                QRect &cropRect,
                                const Qt::GlobalColor *fillRemainClr,
                                const QPoint *eventLoc,
                                const QPoint *eventLocOff)
{
    QImage cropImg;
    QRect boundedCropRect;  // Crop rect that cannot be larger than imgToCrop.
    QPoint eventLocInImgToCropCoords;
    QPoint *cropRectCenterPt = nullptr;

    // Do we have to calculate the cropRect position?
    if (cropRect.x() < 0 || cropRect.y() < 0) {
        // Did the caller not provide eventLoc?
        if (eventLoc == nullptr) {
            throw std::runtime_error("Error: eventLoc is nullptr when cropRect origin should be calculated based off of it.");
        }

        // Do we need to calculate the coordinates for eventLoc using eventLocOff offset?
        eventLocInImgToCropCoords = (eventLocOff != nullptr) ? BoundsChecker::offsetPoint(*eventLoc, *eventLocOff, true)
                                                             : *eventLoc;
        cropRectCenterPt = &eventLocInImgToCropCoords;
    }

    // Refine the crop rectangle to be within the boundaries of imgToCrop and centered around cropRectCenterPt.
    QSize imgToCropSize = imgToCrop.size();
    boundedCropRect = BoundsChecker::refineCropBounds(cropRect, &imgToCropSize, cropRectCenterPt);

    // Do the crop!
    cropImg = (fillRemainClr != nullptr) ? QImage(cropRect.size(), imgToCrop.format())         // May be larger than imgToCrop
                                         : QImage(boundedCropRect.size(), imgToCrop.format()); // Cannot be larger than imgToCrop
    if (fillRemainClr != nullptr) cropImg.fill(*fillRemainClr);
    QPainter painter(&cropImg);
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    // Copy our ROI in the imgToCrop to cropImg. If it is smaller than cropImg, then we center it in it.
    int imgToCropX = (cropRect.width() - boundedCropRect.width()) / 2;
    int imgToCropY = (cropRect.height() - boundedCropRect.height()) / 2;
    //RecordImgDbg::printRect(cropRect);
    //RecordImgDbg::printRect(boundedCropRect);
    painter.drawImage(imgToCropX, imgToCropY, imgToCrop, boundedCropRect.x(), boundedCropRect.y(),
                      boundedCropRect.width(), boundedCropRect.height());

    return cropImg;
}


QImage RecordImageUtil::markEventLoc(const QImage &imgToMark,
                                     const QPoint &eventLoc,
                                     const QPoint *eventLocOff,
                                     const QColor &markClr)
{
    QImage markedImg = imgToMark;
    QPainter painter(&markedImg);

    // Do we need to offset the event location?
    QPoint eventLocImgCoords = (eventLocOff != nullptr) ? BoundsChecker::offsetPoint(eventLoc, *eventLocOff, true)
                                                        : eventLoc;

    painter.setBrush(QBrush(markClr));
    painter.drawEllipse(eventLocImgCoords.x(),
                        eventLocImgCoords.y(),
                        3, 3);

    return markedImg;
}


// Do not allow the static class to be instantiated!
RecordImageUtil::RecordImageUtil() {}
RecordImageUtil& RecordImageUtil::operator=(const RecordImageUtil &rhs) { return *this; }
