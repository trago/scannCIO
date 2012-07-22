#include <iostream>
#include <vector>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "scann.h"

using namespace cv;
using namespace std;

Mat changeBrightContrast(Mat image, float bright, float contrast)
{
  Mat newImage = Mat::zeros(image.size(), image.type());
  for(int i=0; i<image.rows; i++)
    for(int j=0; j<image.cols; j++)
      for(int c=0; c<3; c++){
        newImage.at<Vec3b>(i,j)[c] =
            saturate_cast<uchar>(contrast*image.at<Vec3b>(i,j)[c] + bright);
      }
  return newImage;
}

int main() {

    // Creamos una instancia de Camara
    Camara WebCam;

    Mat imagen;
    Mat imbin;
    Mat imbin1;

    // Elegimos el Dispositivo con el cual trabajar
    WebCam.setDispositivo(0);

    // Capturamos la imagen
    WebCam.Capture( imagen );
    //imwrite("10MPX01.jpg", imagen);
    //imagen = imread("10MPX01.jpg");
    namedWindow("Obtenida de la camara..", WINDOW_NORMAL);
    imshow("Obtenida de la camara..", imagen);

    // Recortar imagen
    WebCam.ExtraeHoja(imagen);
    namedWindow("Recortada..", WINDOW_NORMAL);
    imshow("Recortada..", imagen);


    /*
    // Filtro Canny
    Canny( imagen, imbin, 125, 350 );
    threshold( imagen, imbin1, 128, 255, THRESH_BINARY );
    imshow("Canny", imbin1);
    imwrite( "canny.jpg", imbin1 );
    */

    /*
    // Filto Threshold adaptativo
    int block_size = 91; //Debe ser un valor par <-?? Par o inpar??
    double C = 12.5;
    adaptiveThreshold(imagen, imbin1, 256, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY_INV,
                      block_size, C);

    cvNamedWindow( "Threshold", CV_WINDOW_NORMAL );
    imshow("Threshold", imbin1);
    */    

    //namedWindow("final", WINDOW_NORMAL);
    //imshow("final", final);

    waitKey();
    cvDestroyAllWindows();
    return 0;
}
