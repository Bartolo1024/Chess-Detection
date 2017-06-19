#include "fieldfinder.h"

void findMaxXY(int &maxY, int &minY, int &maxX, int &minX, vector< vector< Point > >  &contours){
    vector<cv::Rect> boundRect(contours.size());
    maxY = 0, minY = INT_MAX, maxX = 0, minX = INT_MAX;
    for(size_t i=0; i < contours.size(); i++){
        boundRect[i] = boundingRect( Mat(contours[i]) );
        Rect boundRectangle = boundRect[i];
        int actualY = boundRectangle.y;
        int actualX = boundRectangle.x;
        if(actualY > maxY){
            maxY = actualY;
        }
        if(actualY < minY){
            minY = actualY;
        }
        if(actualX > maxX){
            maxX = actualX;
        }
        if(actualX < minX){
            minX = actualX;
        }
    }
}

bool deleteTooNearXPoint(int prevX, int widthOfSquare, vector< vector< Point > >  &contours){
    vector<cv::Rect> boundRect(contours.size());
    int maxX = 0, index = 0;
    for(size_t i=0; i < contours.size(); i++){
        boundRect[i] = boundingRect( Mat(contours[i]) );
        Rect boundRectangle = boundRect[i];
        int actualX = boundRectangle.x;
        if(actualX > maxX){
            maxX = actualX;
            index = i;
        }
    }
    if(prevX - maxX < 0.6*widthOfSquare){
        contours.erase(contours.begin() + index);
        return true;
    }
    return false;
}

Mat findFieldsAndDrawItOnImage(Mat cornersBeforeHarrisDetestor, double allowedError, Mat scaleInputMatrix)
{
    int const LINE_INTENSITY = 255;
    //find contours
    vector< vector< Point > > contours;
    vector< vector< Point > > contoursTotal;
    vector< vector< Point > > contoursY;
    vector< vector< Point > > contoursX;
    findContours(cornersBeforeHarrisDetestor, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<cv::Rect> boundRect(contours.size());
    int maxY = 0, minY = INT_MAX, maxX = 0, minX = INT_MAX;
    findMaxXY(maxY, minY, maxX, minX, contours);
    double widthOfBoard = sqrt((maxX-minX)*(maxX-minX));
    double widthOfSquare = widthOfBoard/8;
    double heightOfBoard = sqrt((maxY-minY)*(maxY-minY));
    double heightOfSquare = heightOfBoard/8;
    allowedError = 0.5*heightOfSquare;
    for(int r = 0; r < 9; r++){
        findMaxXY(maxY, minY, maxX, minX, contours);
        contoursY.clear();
        int ind = 0;
        for(vector< vector< Point > >::iterator it = contours.begin(); it != contours.end();){
            boundRect[ind] = boundingRect( Mat( *it ));
            Rect boundRectangle = boundRect[ind];
            int actualY = boundRectangle.y;
            if(actualY < maxY + allowedError && actualY > maxY - allowedError){
                contoursY.push_back(*it);
                contoursTotal.push_back(*it);
                contours.erase(it);
            }
            else{
                ++it;
                ind++;
            }
        }

        Vec4f lineY;
        vector< Point2f > points;
        for(size_t i = 0; i < contoursY.size(); i++){
            vector< Point > add = contoursY[i];
            vector<Point2f> vec;
            for(int i = 0; i < add.size(); i++){
                vec.push_back((Point2f(add.at(i).x, add.at(i).y)));
            }
            points.insert(points.end(), vec.begin(), vec.end());
        }
        if(points.size() > 1) fitLine(points, lineY, CV_DIST_L2, 0, 0.01, 0.01);
        int m = 1000;
        line( scaleInputMatrix, Point2f(lineY[2]-lineY[0]*m, lineY[3]-lineY[1]*m), Point2f(lineY[2]+lineY[0]*m, lineY[3]+lineY[1]*m), Scalar(LINE_INTENSITY,LINE_INTENSITY,LINE_INTENSITY), 1, CV_AA);
    }

    //RESET CONTOURS
    contours.insert(contours.end(), contoursTotal.begin(), contoursTotal.end());
    allowedError = 0.4*widthOfSquare;

    int prevXVal = 0;
    for(int w = 0; w < 9; w++){
        if(w != 0){
            while(deleteTooNearXPoint(prevXVal, widthOfSquare, contours)){
                cout<<"too near points"<< prevXVal;
            }
        }
        findMaxXY(maxY, minY, maxX, minX, contours);
        contoursX.clear();
        int ind = 0;
        for(vector< vector< Point > >::iterator it = contours.begin(); it != contours.end();){
            boundRect[ind] = boundingRect( Mat( *it ));
            Rect boundRectangle = boundRect[ind];
            int actualX = boundRectangle.x;
            if(actualX < maxX + allowedError && actualX > maxX - allowedError){
                contoursX.push_back(*it);
                contoursTotal.push_back(*it);
                contours.erase(it);
            }
            else{
                ++it;
                ind++;
            }
        }
        Vec4f lineX;
        vector< Point2f > points;
        for(size_t i = 0; i < contoursX.size(); i++){
            vector< Point > add = contoursX[i];
            vector<Point2f> vec;
            for(int i = 0; i < add.size(); i++){
                vec.push_back((Point2f(add.at(i).x, add.at(i).y)));
            }
            points.insert(points.end(), vec.begin(), vec.end());
        }

        if(points.size() > 1) fitLine(points, lineX, CV_DIST_L2, 0, 0.01, 0.01);
        else cout<<" not enough points"<<w<<endl;
        int m = 1000;
        line( scaleInputMatrix, Point2f(lineX[2]-lineX[0]*m, lineX[3]-lineX[1]*m), Point2f(lineX[2]+lineX[0]*m, lineX[3]+lineX[1]*m), Scalar(LINE_INTENSITY,LINE_INTENSITY,LINE_INTENSITY), 1, CV_AA);
        prevXVal = maxX;
    }
    return scaleInputMatrix;
}

vector< vector<Mat> > sortMats(vector<pair<Mat, Rect>> matsWithRectangles)
{
    vector< vector<Mat> > sortedMats;
    for(int r = 0; r < 8; r++){
        vector<pair<Mat, Rect>> row;
        vector<Mat> matsRow;
        for(int c = 0; c < 8; c++){
           int maxIndex = 0, actualIndex = 0;
           pair<Mat, Rect> maxYMat = matsWithRectangles[0];
           for(pair<Mat, Rect> mat : matsWithRectangles){
               Rect rect = mat.second, maxMatRect = maxYMat.second;
               if(rect.y > maxMatRect.y){
                   maxYMat = mat;
                   maxIndex = actualIndex;
               }
               actualIndex++;
           }
           row.push_back(maxYMat);
           matsWithRectangles.erase(matsWithRectangles.begin()+maxIndex);
        }
        for(int i = 0; i < row.size(); i++){
            for(int j = 1; j < row.size(); j++){
                pair<Mat, Rect> actualR = row[j], prevR = row[j - 1], tmp;
                if(actualR.second.x < prevR.second.x){
                    tmp = row[j];
                    row[j] = row[j - 1];
                    row[j - 1] = tmp;
                }
            }
        }
        for(pair<Mat, Rect> matRect : row){
            matsRow.push_back(matRect.first);
        }
        sortedMats.push_back(matsRow);
    }
    return sortedMats;
}

vector< vector<Rect> > sortRectangles(vector<pair<Mat, Rect>> matsWithRectangles)
{
    vector< vector<Rect> > sortedRects;
    for(int r = 0; r < 8; r++){
        vector<pair<Mat, Rect>> row;
        vector<Rect> rectRow;
        for(int c = 0; c < 8; c++){
           int maxIndex = 0, actualIndex = 0;
           pair<Mat, Rect> maxYMat = matsWithRectangles[0];
           for(pair<Mat, Rect> mat : matsWithRectangles){
               Rect rect = mat.second, maxMatRect = maxYMat.second;
               if(rect.y > maxMatRect.y){
                   maxYMat = mat;
                   maxIndex = actualIndex;
               }
               actualIndex++;
           }
           row.push_back(maxYMat);
           matsWithRectangles.erase(matsWithRectangles.begin()+maxIndex);
        }
        for(int i = 0; i < row.size(); i++){
            for(int j = 1; j < row.size(); j++){
                pair<Mat, Rect> actualR = row[j], prevR = row[j - 1], tmp;
                if(actualR.second.x < prevR.second.x){
                    tmp = row[j];
                    row[j] = row[j - 1];
                    row[j - 1] = tmp;
                }
            }
        }
        for(pair<Mat, Rect> matRect : row){
            rectRow.push_back(matRect.second);
        }
        sortedRects.push_back(rectRow);
    }
    return sortedRects;
}
