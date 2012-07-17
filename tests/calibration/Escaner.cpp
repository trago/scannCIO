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

    // Elegimos el Dispositivo con el cual trabajar
    WebCam.setDispositivo(0);

    // Capturamos la imagen
    WebCam.Capture( imagen );
    imshow("Imagen", imagen);

    imagen = imread("pruebaHD.jpg",0);
    imshow("Original", imagen);

    Mat imbin;
    Canny( imagen, imbin, 125, 350 );
    imshow("Canny", imbin);

    Mat imbin1;
    //threshold( imagen, imbin1, 128, 255, THRESH_BINARY_INV );
    int block_size = 91; //Debe ser un valor par
    double C = 12.5;
    adaptiveThreshold(imagen, imbin1, 256, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY_INV,
                      block_size, C);
    imshow("Threshold", imbin1);


    cvWaitKey();
    return 0;
}
