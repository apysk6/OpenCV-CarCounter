#include "Detector.h"

Point Detector::findCenterPoint(vector<Point> contour) {
	Moments oMoments = moments(contour);
	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dM00 = oMoments.m00;

	int pos_x = dM10 / dM00;
	int pos_y = dM01 / dM00;

	return Point(pos_x, pos_y);
}

Point* Detector::makeCrossingLine(int cameraFrameCols) {
	Point* crossingLine = new Point[2];

	crossingLine[0].x = 0;
	crossingLine[0].y = this->horizontalLinePosition;

	crossingLine[1].x = cameraFrameCols - 1;
	crossingLine[1].y = this->horizontalLinePosition;

	return crossingLine;
}

void Detector::detectCars() {
	Mat countour = this->foreground.clone();
	findContours(countour, contours, RETR_LIST, CHAIN_APPROX_NONE);

	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		Rect contour = boundingRect(contours[i]);
		Point centerPoint = findCenterPoint(contours[i]);

		// Remove small contours and object (like motorbike).
		if (area < 2500)
			continue;

		if (centerPoint.x < 300)
			continue;

		rectangle(cameraFrame, contour, Scalar(220, 220, 220), 1);
		int intHorizontalLinePosition = this->horizontalLinePosition;

		if (centerPoint.y <= this->horizontalLinePosition) {

			if (intHorizontalLinePosition - 20 < centerPoint.y && centerPoint.y < intHorizontalLinePosition + 20) {
				crossedTime = clock();
				double timeFromCross = (crossedTime - start) / (CLOCKS_PER_SEC / 1000);

				if ((!isInit && timeFromCross < 1000 && (centerPoint.x > crossedCenterPoint.x - 50 && centerPoint.x < crossedCenterPoint.x + 50) && (centerPoint.y < crossedCenterPoint.y + 50 && centerPoint.y > centerPoint.y - 50))) {
					continue;
				}
				crossedCenterPoint = centerPoint;
				this->carCount = this->carCount + 1;
				start = clock();
			}

			continue;
		}
	}
}

void Detector::disableInit() {
	this->isInit = false;
}

Mat Detector::getCameraFrame() {
	return this->cameraFrame;
}

Mat Detector::getForeground() {
	return this->foreground;
}

void Detector::setForeground(Mat foreground) {
	this->foreground = foreground;
}

int Detector::getCarCount() {
	return this->carCount;
}

void Detector::setHorizontalLinePosition(int horizontalLinePosition) {
	this->horizontalLinePosition = horizontalLinePosition;
}

void Detector::setCameraFrame(Mat cameraFrame) {
	this->cameraFrame = cameraFrame;
}
