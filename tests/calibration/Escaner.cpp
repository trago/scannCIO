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

    // Elegimos el Dispositivo con el cual trabajar
    WebCam.setDispositivo(0);

    // Capturamos la imagen
    //WebCam.Capture( imagen );
    //imshow("Imagen", imagen);

    imagen = imread("pruebaHD2.jpg", 0);
    cvNamedWindow( "Original", CV_WINDOW_NORMAL );
    imshow("Original", imagen);

    // Filtro Canny
    Canny( imagen, imbin, 125, 350 );
    threshold( imbin, imbin1, 128, 255, THRESH_BINARY_INV );
    //imshow("Canny", imbin);
    imwrite( "canny.jpg", imbin1 );

    int block_size = 91; //Debe ser un valor par <-?? Par o inpar??
    double C = 12.5;
    adaptiveThreshold(imagen, imbin1, 256, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY_INV,
                      block_size, C);
    cvNamedWindow( "Threshold", CV_WINDOW_NORMAL );
    imshow("Threshold", imbin1);


    // Extrayendo bordes
    vector< vector<Point> > bordes;
    findContours( imbin1, bordes, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    // Dibujando los contornos
    Mat imborders( imagen.size(), CV_8U, Scalar(255) ); // Imagen binaria "Blanca" del tamaño de la original
    drawContours( imborders, bordes, -1, Scalar(0), 5 );
    cvNamedWindow( "Bordes", CV_WINDOW_NORMAL );
    imshow("Bordes", imborders);
    //imwrite( "ztest.jpg", imborders );



    cvWaitKey();
    cvDestroyAllWindows();
    return 0;
}
