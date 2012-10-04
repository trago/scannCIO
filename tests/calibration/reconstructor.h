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
        cv::Point points[4];

        // Variables de la Matriz
        float outX1, outX2, outX3, outX4, outY1, outY2, outY3, outY4;
        int inHeight, inWidth, inStep, inChannels;
        int outHeight, outWidth, outStep, outChannels;
        uchar *inData;
        uchar *outData;
        int constru_Height, constru_Width, constru_Step, constru_Channels;
        uchar *constru_Data;

        // Funciones
        void setPoint(int x, int y);
        void compute_matrix();
};

#endif // RECONSTRUCTOR_H
