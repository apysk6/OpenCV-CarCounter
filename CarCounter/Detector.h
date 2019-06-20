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
#include <string>

using namespace cv;
using namespace std;

class Detector {

private:
	Mat cameraFrame;
	Mat foreground;
	vector<vector<Point>> contours;
	int horizontalLinePosition;
	int carCount = 0;
	Point crossedCenterPoint;
	bool isInit = true;

public:
	Point findCenterPoint(vector<Point> contour);

	Point* makeCrossingLine(int cameraFrameCols);

	Mat getCameraFrame();

	Mat getForeground();

	void disableInit();

	int getCarCount();

	void setCameraFrame(Mat cameraFrame);

	void setForeground(Mat foreground);

	void setHorizontalLinePosition(int horizontalLinePosition);

	void detectCars();
};



