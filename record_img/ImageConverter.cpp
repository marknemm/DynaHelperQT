#include "ImageConverter.h"
#include <QDebug>


Mat ImageConverter::convertQImageToMat(QImage &srcImg)
{
    switch (srcImg.format()) {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        return Mat(srcImg.height(), srcImg.width(), CV_8UC4, const_cast<uchar*>(srcImg.bits()), static_cast<size_t>(srcImg.bytesPerLine()));
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB32:
    case QImage::Format_RGB888:
    {
        if (srcImg.format() == QImage::Format_RGB32) {
            srcImg = srcImg.convertToFormat(QImage::Format_RGB888);
        }
        //srcImg = srcImg.rgbSwapped();
        // Don't care if color channels are not right in CV Mat. We do not want to screw up original QImage color channels.

        return Mat(srcImg.height(), srcImg.width(), CV_8UC3, const_cast<uchar*>(srcImg.bits()),
                   static_cast<size_t>(srcImg.bytesPerLine())).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        return Mat(srcImg.height(), srcImg.width(), CV_8UC1, const_cast<uchar*>(srcImg.bits()),
                   static_cast<size_t>(srcImg.bytesPerLine()));
    }

    default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << srcImg.format();
    }

    return Mat();
}


Mat ImageConverter::convertRGBToGray(const Mat &srcImg)
{
    Mat destImg;
	cvtColor(srcImg, destImg, CV_RGB2GRAY);
    return destImg;
}


Mat ImageConverter::convertGrayToRGB(const Mat &srcImg)
{
    Mat destImg;
	cvtColor(srcImg, destImg, CV_GRAY2RGB);
    return destImg;
}


Mat ImageConverter::convertGrayToBW(const Mat &srcImg, const uchar *thresh /*=nullptr*/)
{
    Mat destImg;
	if(thresh != nullptr) {
		destImg = (srcImg > *thresh);
	}
	else {
		threshold(srcImg, destImg, 0.0, 255.0, (THRESH_BINARY | THRESH_OTSU));
	}
    return destImg;
}


Mat ImageConverter::convertRGBToBW(const Mat &srcImg, const uchar *thresh /*=nullptr*/)
{
    Mat destImg;
    destImg = convertRGBToGray(srcImg);
    destImg = convertGrayToBW(destImg, thresh);
    return destImg;
}


Mat ImageConverter::convertToGrayIfOtherFormat(const Mat &srcImg)
{
    Mat destImg;
	if(srcImg.channels() == 3) { // If image is not in grayscale format:
        destImg = convertRGBToGray(srcImg);
	}
    else {
        destImg = srcImg;
    }
    return destImg;
}
