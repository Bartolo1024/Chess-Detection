#include "board.h"

Board::Board(std::vector<std::vector<cv::Mat> > vectorOfSubimages)
{
    subimages = vectorOfSubimages;//board rotate 180*
    //set up size
    for (std::vector<Figure*> row : figures){
        for(Figure* fig : row){
            fig = new Figure(empty, none);
        }
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
                    if(i==0 && j == 4)cv::imshow(str, row[j]);
                    checkBlackSquare(row[j]);
                    ss<<'c';
                    if(i==0 && j == 4)cv::imshow(str, row[j]);
                }
            }
            else{
                if(j%2==1){
                    //process function
                    std::ostringstream ss;
                    ss << i;
                    ss << j;
                    std::string str = ss.str();
                    if((i==7 || i==3) && j == 3)cv::imshow(str, row[j]);
                    checkBlackSquare(row[j]);
                    ss<<'c';
                    if((i==7 || i==3) && j == 3)cv::imshow(str, row[j]);
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

void Board::checkWhiteSquare(cv::Mat square)
{
    cv::threshold(square, square, 20, 255, cv::THRESH_BINARY);
}

void Board::checkBlackSquare(cv::Mat square)
{
    cv::Canny(square, square, 230, 250, 5);
    cv::dilate(square, square, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
    cv::erode(square, square, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));
    //cv::threshold(square, square, 25, 255, cv::THRESH_BINARY);
}
