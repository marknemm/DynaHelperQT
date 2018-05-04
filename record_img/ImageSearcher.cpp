#include "ImageSearcher.h"
#include "RecordImgDbg.h"
#include "ImageConverter.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"


/**
--- DESCRIPTION ---
Utilizes template matching to search for a target image within the screenshot image.
--- INPUTS ---
targetImg: The (grayscale) image to search for within the screenshot image.
screenshot: The (grayscale) screenshot image.
targetOrgPredict: The predicted cv::Point of the origin (top-left corner coordinates) of the target image within the screenshot image.
matchMethod: (OPTIONAL default = CV_TM_CCORR) The mathematical method used for template matching.
--- OUTPUT ---
The origin (top-left corner coordinates) of the found target image within the screenshot image. If no image was found, then
the returned cv::Point will contain (-1, -1).
*/
cv::Point ImageSearcher::templateMatchSearch(const Mat &targetImg, const Mat &screenshot, const cv::Point &targetOrgPredict, int matchMethod /*=CV_TM_CCOEFF*/)
{
    //const int ROI_SCALE = 10;
	cv::Point minLoc, maxLoc, foundTargetOrg(-1, -1);
	int resultRows, resultCols;
	double minVal, maxVal;
	cv::Rect screenshotROI;
	Mat result;

	/* First check only locally over the screenshot (Target is likely moved to near vacinity)
	   and zoom out by a LOCALITY_FACTOR if not found in first loop iteration: */
	for(int zoomOutFactor = 1 ; zoomOutFactor <= 2 ; zoomOutFactor++) {
		//screenshotROI = clacRoiAroundTargetCenter(targetOrgPredict, targetImg, screenshot, ROI_SCALE * zoomOutFactor);
		screenshotROI = cv::Rect(0, 0, screenshot.cols, screenshot.rows);
		resultRows = (screenshotROI.height - targetImg.rows + 1);
		resultCols = (screenshotROI.width - targetImg.cols + 1);
		result.create(resultRows, resultCols, CV_32FC1);

		matchTemplate(screenshot(screenshotROI), targetImg, result, matchMethod);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
		// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if(matchMethod == CV_TM_SQDIFF || matchMethod == CV_TM_SQDIFF_NORMED) {
			foundTargetOrg = minLoc;
		}
		else {
			foundTargetOrg = maxLoc;
		}
		break;
	}

	// Factor roi target origin result back into total screenshot:
	foundTargetOrg.x += screenshotROI.x;
	foundTargetOrg.y += screenshotROI.y;
    Mat screenshotRGB = ImageConverter::convertGrayToRGB(screenshot);
	rectangle(screenshotRGB, screenshotROI, Scalar(255, 0, 0), 3);
	rectangle(screenshotRGB, foundTargetOrg, cv::Point(foundTargetOrg.x + targetImg.cols, foundTargetOrg.y + targetImg.rows), Scalar(0, 0, 255), 3);
	rectangle(screenshotRGB, targetOrgPredict, cv::Point(targetOrgPredict.x + targetImg.cols, targetOrgPredict.y + targetImg.rows), Scalar(0, 255, 0), 5);
	RecordImgDbg::displayImg(screenshotRGB);

	return foundTargetOrg;
}


cv::Point ImageSearcher::featureBasedSearch(const Mat &targetImg, const Mat &screenshot, const cv::Point &targetOrgPredict)
{
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	std::vector<DMatch> matches, good_matches;
	std::vector<Point2f> obj, scene, obj_corners(4), scene_corners(4);
	Mat descriptors_object, descriptors_scene, img_matches, homography;
	FlannBasedMatcher matcher;
	double dist, max_dist(0), min_dist(100);
	const int minHessian = 10;

	//-- Step 1: Detect the keypoints using SURF Detector
	FastFeatureDetector detector(minHessian);
	detector.detect(targetImg, keypoints_object);
	detector.detect(screenshot, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;
	extractor.compute(targetImg, keypoints_object, descriptors_object);
	extractor.compute(screenshot, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	matcher.match(descriptors_object, descriptors_scene, matches);

	//-- Quick calculation of max and min distances between keypoints
	for(int i = 0 ; i < descriptors_object.rows ; i++) {
		dist = matches[i].distance;
		if(dist < min_dist) { min_dist = dist; }
		if(dist > max_dist) { max_dist = dist; }
	}

	//-- Draw only "good" matches (i.e. whose distance is less than 3 * min_dist )
	for(int i = 0 ; i < descriptors_object.rows ; i++) {
		//if(matches[i].distance <= (3 * min_dist)) {
		good_matches.push_back(matches[i]);
	}

	drawMatches(targetImg, keypoints_object, screenshot, keypoints_scene,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
    for(uint i = 0 ; i < good_matches.size() ; i++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	// Need at least 4 pairs to find match!
	if(obj.size() >= 4 && scene.size() >= 4) {
		homography = findHomography(obj, scene, CV_RANSAC);

		//-- Get the corners from the image_1 ( the object to be "detected" )
		obj_corners[0] = cvPoint(0, 0);
		obj_corners[1] = cvPoint(targetImg.cols, 0);
		obj_corners[2] = cvPoint(targetImg.cols, targetImg.rows);
		obj_corners[3] = cvPoint(0, targetImg.rows);
		perspectiveTransform(obj_corners, scene_corners, homography);

		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		line(img_matches, scene_corners[0] + Point2f(targetImg.cols, 0), scene_corners[1] + Point2f(targetImg.cols, 0), Scalar(255, 255, 255), 4);
		line(img_matches, scene_corners[1] + Point2f(targetImg.cols, 0), scene_corners[2] + Point2f(targetImg.cols, 0), Scalar(0, 255, 0), 4);
		line(img_matches, scene_corners[2] + Point2f(targetImg.cols, 0), scene_corners[3] + Point2f(targetImg.cols, 0), Scalar(255, 0, 0), 4);
		line(img_matches, scene_corners[3] + Point2f(targetImg.cols, 0), scene_corners[0] + Point2f(targetImg.cols, 0), Scalar(0, 0, 255), 4);

		//-- Show detected matches
		RecordImgDbg::displayImg(img_matches, "Feature Based Search");
		return scene_corners[0];
	}

	return cv::Point(-1, -1); // Nothing could be found!
}


/**
--- DESCRIPTION ---
Calculates the roi around the center of the predicted location of the target image (for initially searching over a smaller localized area).
--- INPUTS ---
targetOrgPredict: The predicted origin (top-left corner coordinates) of the target image within the screenshot image.
targetImg: The target image that will be searched for.
screenshot: The screenshot in which to search for the target image.
roiScale: (OPTIONAL default = 3) The scale of the calculated roi in comparison to the target image size. For example, setting the roiScale = 3
will cause the roi to have a width of 3x the targetImg width and a height of 3x the targetImg height as long as the roi centered around the
predicted location of the target image fits within the screenshot boundaries. If it does not, then the maximum possible width and height are
used instead.
--- OUTPUT ---
The roi cv::Rectangle.
*/
cv::Rect ImageSearcher::clacRoiAroundTargetCenter(const cv::Point &targetOrgPredict, const Mat &targetImg, const Mat &screenshot, int roiScale /*=3*/)
{
	int roiX, roiY, roiW, roiH;

	roiW = roiScale * targetImg.cols;
	roiH = roiScale * targetImg.rows;

	roiX = (targetOrgPredict.x + (targetImg.cols / 2) - (roiW / 2));
	roiY = (targetOrgPredict.y + (targetImg.rows / 2) - (roiH / 2));

	// Check if in bounds for X and Y:
	roiX = roiX >= 0 ? roiX : 0;
	roiY = roiY >= 0 ? roiY : 0;

	// Check if in bounds for W and H:
	roiW = (roiX + roiW < screenshot.cols) ? roiW : screenshot.cols - roiX;
	roiH = (roiY + roiH < screenshot.rows) ? roiH : screenshot.rows - roiY;

	return cv::Rect(roiX, roiY, roiW, roiH);
}
