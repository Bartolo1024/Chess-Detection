#ifndef FIELDFINDER_H
#define FIELDFINDER_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat findFieldsAndDrawItOnImage(Mat cornersBeforeHarrisDetestor, double allowedError,Mat  scaleInputMatrix);
vector< vector<Mat> > sortMats(vector<pair<Mat, Rect>> matsWithRectangles);
vector<vector<Rect> > sortRectangles(vector<pair<Mat, Rect>> matsWithRectangles);
#endif // FIELDFINDER_H
