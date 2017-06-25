#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "fieldfinder.h"
#include "board.h"
#include "figure.h"

using namespace std;
using namespace cv;

double distance(int x1, int y1, int x2, int y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int main(int argc, char** argv)
{
    int scale = 4;
    int allowedError;

    Mat scaleInputMatrix, cannyMat, grayMatrix, harrisdst, dst_norm, cornersAfterHarrisDetestor;

    // Load picture and set size
    string path = string("data/2.jpg");
    Mat input = imread(path);
    resize(input, scaleInputMatrix, Size(input.cols/scale, input.rows/scale)); ///
    cvtColor(scaleInputMatrix, grayMatrix, COLOR_BGR2GRAY);

    //Canny edges detedtion
    GaussianBlur(grayMatrix, grayMatrix, Size(3,3), 0.42);//042
    Canny(grayMatrix, cannyMat, 75, 160, 3);

    //hough
    Mat1f cdst(cannyMat.rows, cannyMat.cols, 1.0f);
    vector<Vec4i> lines;
    //hough
    HoughLinesP(cannyMat, lines, 0.5, CV_PI/360, 60, 70, 12);
    for( size_t i = 0; i < lines.size(); i++ )
      {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
      }

    //////////////////////////////harris
    cornerHarris( cdst, harrisdst, 8, 3, 0.02, BORDER_DEFAULT);
    normalize( harrisdst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, cornersAfterHarrisDetestor );
    threshold(cornersAfterHarrisDetestor, cornersAfterHarrisDetestor, 100, 255, THRESH_BINARY);
    for( int j = 0; j < dst_norm.rows ; j++ )
         { for( int i = 0; i < dst_norm.cols; i++ )
              {
                if( (int) dst_norm.at<float>(j,i) > 100 )
                  {
                   circle( cornersAfterHarrisDetestor, Point( i, j ), 1,  Scalar(255,255,255), 2, 8, 0 );
                  }
              }
         }
    /////////////////////////////////

    //MASK OF FIELDS
    Mat mask = Mat(scaleInputMatrix.rows ,scaleInputMatrix.cols, CV_8UC1);
    mask.setTo(Scalar(0));
    mask = findFieldsAndDrawItOnImage(cornersAfterHarrisDetestor, allowedError, mask);
    //threshold(grayMatrix, grayMatrix, 200, 255, THRESH_BINARY);
    vector< vector< Point > > contours;
    findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

    double maxArea = 0;
    int maxContourIndex = 0;
    for(size_t i=0; i < contours.size(); i++){
        if(contourArea(contours[i]) > maxArea){
            maxContourIndex = i;
            maxArea = contourArea(contours[i]);
        }
    }
    cout<<maxContourIndex<<endl;
    contours.erase(contours.begin() + maxContourIndex);
    cout<<contours.size()<<endl;

    //draw contour on firs image
    cvtColor(scaleInputMatrix, grayMatrix, COLOR_BGR2GRAY);
    vector<vector<Mat> > subimages;
    vector<pair<Mat, Rect> > matsWithRects;
    vector<cv::Rect> boundRect(contours.size());
    for(size_t i=0; i < contours.size(); i++){
        boundRect[i] = boundingRect(contours[i]);
        boundRect[i].width -=20;
        boundRect[i].height -=20;
        boundRect[i].x +=10;
        boundRect[i].y +=10;
        if(boundRect[i].width < boundRect[i].height){
            boundRect[i].height = boundRect[i].width;
        }
        if(boundRect[i].height < boundRect[i].width){
            boundRect[i].width = boundRect[i].height;
        }
        matsWithRects.push_back(make_pair(Mat (grayMatrix, boundRect[i]), boundRect[i]));
        //drawContours(scaleInputMatrix, contours, i, Scalar(255, 255, 255), CV_FILLED);
    }

    subimages = sortMats(matsWithRects);

    //Create board with figures
    Board *board = new Board(subimages);
    board->processWhiteSquares();
    board->processBlackSquares();


    std::vector< std::vector <Figure*> > checkedFigures = board->getFigures();
    vector<vector<Rect> > rects = sortRectangles(matsWithRects);

    for (int i = 0; i < subimages.size(); i++){
        for(int j = 0; j < subimages.size(); j++){
            Figure *actualFig = checkedFigures[i][j];
            if(actualFig->isFigureInThisField() == true){
                rectangle(scaleInputMatrix, rects[i][j], Scalar(255),1);
            }
        }
    }

    // Show output*/
    grayMatrix = grayMatrix + cornersAfterHarrisDetestor;
    imshow("output", scaleInputMatrix);
    waitKey(0);

    delete board;
    // Return
    return 0;
}

