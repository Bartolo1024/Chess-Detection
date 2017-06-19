#include "board.h"

Board::Board(std::vector<std::vector<cv::Mat> > vectorOfSubimages)
{
    subimages = vectorOfSubimages;//board rotate 180*
    //set up size
    for (int i = 0; i < subimages.size(); i++){
        std::vector<Figure*> row;
        for(int j = 0; j < subimages.size(); j++){
            Figure *fig = new Figure(empty, none);
            row.push_back(fig);
        }
        figures.push_back(row);
    }
    //showMats();
}

Board::~Board()
{
    for (std::vector<Figure*> row : figures){
        for(Figure* fig : row){
            delete fig;
        }
    }
}

void Board::processWhiteSquares()
{
    for(int i = 0; i < subimages.size(); i++){
        std::vector<cv::Mat>  row = subimages[i];
        for(int j = 0; j < row.size(); j++){
            if(i%2==0){
                if(j%2==1){
                    //process function
                    std::ostringstream ss;
                    ss << i;
                    ss << j;
                    std::string str = ss.str();
                    if(checkWhiteSquare(row[j] , str)){
                        Figure *fig = figures[i][j];
                        fig->setName(unidentified);
                    }
                    //ss << checkWhiteSquare(row[j], str);
                    str = ss.str();
                    //cv::imshow(str, row[j]);
                }
            }
            else{
                if(j%2==0){
                    //process function
                    std::ostringstream ss;
                    ss << i;
                    ss << j;
                    std::string str = ss.str();
                    if(checkWhiteSquare(row[j], str)){
                        Figure *fig = figures[i][j];
                        fig->setName(unidentified);
                    }
                    //cv::imshow(str, row[j]);
                }
            }
        }
    }
}

void Board::processBlackSquares()
{
    for(int i = 0; i < subimages.size(); i++){
        std::vector<cv::Mat>  row = subimages[i];
        for(int j = 0; j < row.size(); j++){
            if(i%2==0){
                if(j%2==0){
                    //process function
                    std::ostringstream ss;
                    ss << i;
                    ss << j;
                    std::string str = ss.str();
                    //if(i==0 && j == 4)cv::imshow(str, row[j]);
                    if(checkBlackSquare(row[j])){
                        Figure *fig = figures[i][j];
                        fig->setName(unidentified);
                    }
                    ss<<'c';
                    str = ss.str();
                    /*if(i==0 && j == 4)*///cv::imshow(str, row[j]);
                }
            }
            else{
                if(j%2==1){
                    std::ostringstream ss;
                    ss << i;
                    ss << j;
                    std::string str = ss.str();
                    //if((i==7 || i==3) && j == 3)cv::imshow(str, row[j]);
                    //process function
                    //std::cout<<i<<j<<" ";
                    if(checkBlackSquare(row[j])){
                        Figure *fig = figures[i][j];
                        fig->setName(unidentified);
                    }
                    ss<<'c';
                    str = ss.str();
                    /*if((i==7 || i==3) && j == 3)*///cv::imshow(str, row[j]);
                }
            }
        }
    }
}

void Board::showMats()
{
    for(int i = 0; i < subimages.size(); i++){
        std::vector<cv::Mat>  row = subimages[i];
        for(int j = 0; j < row.size(); j++){
            std::ostringstream ss;
            ss << i;
            ss << j;
            std::string str = ss.str();
            cv::imshow(str, row[j]);
        }
    }
}

std::vector<std::vector<Figure *> > Board::getFigures()
{
    return figures;
}

bool Board::checkWhiteSquare(cv::Mat square, std::string name)
{
    cv::Mat tophat, blackhat, contrast;
    cv::morphologyEx(square, tophat, cv::MORPH_TOPHAT, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(square.rows/10, square.cols/10)));
    cv::morphologyEx(square, blackhat, cv::MORPH_BLACKHAT, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(square.rows/30, square.cols/30)));
    contrast = square + tophat - blackhat;
    square = contrast;
    cv::GaussianBlur(square, square, cv::Size(3,5), 1);
    cv::Canny(square, square, 50, 150, 3);
    std::vector< std::vector<cv::Point> > contours;
    cv::findContours(square.clone(), contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    int pointsCount = 0;
    for(std::vector<cv::Point> contour :  contours){
        cv::Rect rect = cv::boundingRect(contour);
        if(true){
            pointsCount += contour.size();
        }
    }
    std::cout<<name<<"\t"<<pointsCount<<std::endl;
    cv::imshow(name, square);
    if(pointsCount > 350)   return true;
    return false;
}

bool Board::checkBlackSquare(cv::Mat square)
{
    cv::Mat tophat, blackhat, contrast;
    cv::morphologyEx(square, tophat, cv::MORPH_TOPHAT, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(square.rows/10, square.cols/10)));
    cv::morphologyEx(square, blackhat, cv::MORPH_BLACKHAT, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(square.rows/20, square.cols/20)));
    contrast = square + tophat - blackhat;
    square = contrast;
    cv::GaussianBlur(square, square, cv::Size(3,5), 1);
    cv::Canny(square, square, 90, 170, 3);
    std::vector< std::vector<cv::Point> > contours;
    cv::findContours(square.clone(), contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    int pointsCount = 0;
    for(std::vector<cv::Point> contour :  contours){
        cv::Rect rect = cv::boundingRect(contour);
        if(contour.size() > 0){
            pointsCount += contour.size();
        }
    }
    //std::cout<<pointsCount<<std::endl;
    if(pointsCount > 150)   return true;
    return false;
}
