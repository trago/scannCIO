#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

class reconstructor
{
    public:
        reconstructor(cv::Mat &img);
        ~reconstructor();

        int cont;
        cv::Mat inImg, outImg, recImg;
        cv::Point2f points[4];

        // Funciones
        void setPoint(int x, int y);
};

#endif // RECONSTRUCTOR_H
