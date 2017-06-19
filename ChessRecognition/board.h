#ifndef BOARD_H
#define BOARD_H
#include "figure.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream> // to debug

class Board
{
public:
    Board(std::vector < std::vector <cv::Mat >  > vectorOfSubimages);
    ~Board();
    void processWhiteSquares();
    void processBlackSquares();
    void showMats();
    std::vector< std::vector <Figure*> > getFigures();
private:
    //functions
    bool checkWhiteSquare(cv::Mat square, std::string name);
    bool checkBlackSquare(cv::Mat square);

    //data
    std::vector< std::vector <Figure*> > figures;
    std::vector< std::vector <cv::Mat > > subimages;
};

#endif // BOARD_H
