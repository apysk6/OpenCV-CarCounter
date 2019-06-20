#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Detector.h"
#include <string>

using namespace cv;
using namespace std;

int main() {
	VideoCapture videoCaptureProvider("C:/cars2.mp4");
	Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();
	Detector detector;
	Mat cameraFrame, foreground, background;
	Point* crossingLine;

	namedWindow("Background", 1);
	namedWindow("Camera", 1);

	while (videoCaptureProvider.read(cameraFrame))
	{
		videoCaptureProvider.read(cameraFrame);
		int intHorizontalLinePosition = (int)round((double)cameraFrame.rows * 0.35);
		detector.setHorizontalLinePosition(intHorizontalLinePosition);

		crossingLine = detector.makeCrossingLine(cameraFrame.cols);
		line(cameraFrame, crossingLine[0], crossingLine[1], Scalar(0,0,0), 2);

		detector.setCameraFrame(cameraFrame);

		pMOG2->apply(cameraFrame, foreground);
		pMOG2->setHistory(10);
		pMOG2->setVarThreshold(100);
		pMOG2->setDetectShadows(0);

		morphologyEx(foreground, foreground, cv::MORPH_CLOSE, Mat(), Point(-1, -1), 5);
		morphologyEx(foreground, foreground, cv::MORPH_OPEN, Mat(), Point(-1, -1), 5);

		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		detector.setForeground(foreground);
		detector.detectCars();

		int intFontFace = FONT_HERSHEY_SIMPLEX;
		double dblFontScale = (cameraFrame.rows * cameraFrame.cols) / 300000.0;
		int intFontThickness = (int)round(dblFontScale * 1.5);

		// Put counter on right corner.
		cv::Size textSize = cv::getTextSize(to_string(detector.getCarCount()), intFontFace, dblFontScale, intFontThickness, 0);
		cv::Point ptTextBottomLeftPosition;

		ptTextBottomLeftPosition.x = cameraFrame.cols - 1 - (int)((double)textSize.width * 1.25);
		ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);
		putText(cameraFrame, to_string(detector.getCarCount()), ptTextBottomLeftPosition, intFontFace, dblFontScale, Scalar(255, 207, 0), intFontThickness);

		detector.disableInit();
		imshow("Background", detector.getForeground());
		imshow("Camera", cameraFrame);

		if (waitKey(200) == 27)
			break;
	}

	videoCaptureProvider.release();
	destroyAllWindows();

	return 0;
}