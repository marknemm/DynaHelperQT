#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

#include <QImage>
#include <cv.h>

using namespace cv;

class ImageConverter {

public:

    /**
     * @brief convertQImageToMat
     * Converts a QImage to an OpenCV Mat.
     * @param srcImg The source QImage.
     * @return The OpenCv Mat.
     */
    static Mat convertQImageToMat(QImage &srcImg);
    /**
    --- DESCRIPTION ---
    Converts an RGB (Mat) image to a Grayscale (Mat) image.
    --- INPUTS ---
    srcImg: A reference to the Mat object that will be converted to grayscale.
    --- OUTPUT ---
    The destination Mat that will contain the end product of the conversion.
    */
    static Mat convertRGBToGray(const Mat &srcImg);
    /**
    --- DESCRIPTION ---
    Converts a Grayscale (Mat) image to an RGB (Mat) image.
    --- INPUTS ---
    srcImg: A reference to the Mat object that will be converted to RGB.
    --- OUTPUT ---
    The destination Mat that will contain the end product of the conversion.
    */
    static Mat convertGrayToRGB(const Mat &srcImg);
    /**
    --- DESCRIPTION ---
    Converts a grayscale (Mat) image to a pure black and white (Mat) image.
    --- INPUTS ---srcImg: A reference to the Mat object that will be converted to pure black and white.
    thresh: (OPTIONAL default=128) An byte containing the intensity threshold value for the borderline between black and white.
    --- OUTPUT ---
    The destination Mat that will contain the end product of the conversion.
    */
    static Mat convertGrayToBW(const Mat &srcImg, const uchar *thresh = nullptr);
    /**
    --- DESCRIPTION ---
    Converts an RGB (Mat) image to a pure black and white (Mat) image.
    --- INPUTS ---
    srcImg: A reference to the Mat object that will be converted to pure black and white.
    thresh: (OPTIONAL default=128) An byte containing the intensity threshold value for the borderline between black and white.
    --- OUTPUT ---
    The destination Mat that will contain the end product of the conversion.
    */
    static Mat convertRGBToBW(const Mat &srcImg, const uchar *thresh = nullptr);
    /**
    --- DESCRIPTION ---
    Converts an image to a grayscale (Mat) image if it is not already in 8-bit grayscale format.
    --- INPUTS ---
    srcImg: A reference to the Mat object that will be converted to grayscale if not already in 8-bit grayscale format.
    --- OUTPUT ---
    The destination Mat that will contain the end product of the conversion.
    */
    static Mat convertToGrayIfOtherFormat(const Mat &srcImg);
};


#endif
