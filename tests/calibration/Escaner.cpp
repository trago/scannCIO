#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "scann.h"

using namespace cv;
using namespace std;

int main() {

    // Creamos una instancia de Camara
    Camara WebCam;
    Mat imagen;
    Mat imbin;
    Mat imbin1;
    Mat HSV;

    // Elegimos el Dispositivo con el cual trabajar
    WebCam.setDispositivo(0);

    // Capturamos la imagen
    WebCam.Capture( imagen );

    //imagen = imread("pruebaHD2.jpg", 0);
    cvNamedWindow( "Original", CV_WINDOW_NORMAL );
    imshow("Original", imagen);

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

    // Threshold
    threshold( imagen, imbin1, 50, 255, THRESH_BINARY );
    cvNamedWindow( "Threshold", CV_WINDOW_NORMAL );
    imshow("Threshold", imbin1);

    cvtColor(imbin1, HSV, CV_BGR2HSV);

    Mat H( HSV.size(), CV_8U, 1 );
    Mat S( HSV.size(), CV_8U, 1 );
    Mat V( HSV.size(), CV_8U, 1 );

    cvTranspose(&HSV, &HSV);

    cvSplit( HSV, H, S, V, NULL );

    cvNamedWindow( "HSV", CV_WINDOW_NORMAL );
    cvNamedWindow( "H", CV_WINDOW_NORMAL );
    cvNamedWindow( "S", CV_WINDOW_NORMAL );
    cvNamedWindow( "V", CV_WINDOW_NORMAL );

    imshow( "HSV", HSV );
    imshow( "H", HSV );
    imshow( "S", HSV );
    imshow( "V", HSV );


/*
    // Extrayendo bordes
    vector< vector<Point> > bordes;
    findContours( imbin1, bordes, CV_RETR_TREE, CV_CHAIN_APPROX_NONE );

    // Dibujando los contornos
    Mat imborders( imagen.size(), CV_8U, Scalar(255) ); // Imagen binaria "Blanca" del tamaño de la original
    //drawContours( imborders, bordes, -1, Scalar(0), 5 );
    drawContours( imborders, bordes, 5, Scalar(0), 1 );

    // Shapes Descriptors
    Rect r0 = boundingRect( Mat(bordes[5]) );
    rectangle( imborders, r0, Scalar(0), 2 );

    cvNamedWindow( "Bordes", CV_WINDOW_NORMAL );
    imshow("Bordes", imborders);
    //imwrite( "ztest.jpg", imborders );

    //printf("%d",bordes.size());
*/
    cvWaitKey();
    cvDestroyAllWindows();
    return 0;
}
