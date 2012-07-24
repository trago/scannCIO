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

int main() {

    // Creamos una instancia de Camara
    Camara WebCam;

    Mat imagen;
    Mat imbin;
    Mat imbin1;

    // Elegimos el Dispositivo con el cual trabajar
    WebCam.setDispositivo(1);

    // Capturamos la imagen
    //WebCam.Capture( imagen );
    //imwrite("30MPX01.jpg", imagen);
    imagen = imread("10MPX01.jpg");
    namedWindow("Obtenida de la camara..", WINDOW_NORMAL);
    imshow("Obtenida de la camara..", imagen);

    // Recortar imagen
    WebCam.ExtraeHoja(imagen);
    //WebCam.rotarImagen(imagen, 90);
    namedWindow("Recortada..", WINDOW_NORMAL);
    imshow("Recortada..", imagen);

    Mat im2;
    WebCam.ProcesaImagen( imagen, im2 );
    namedWindow("Editada..", WINDOW_NORMAL);
    imshow("Editada..", im2);



    waitKey();
    cvDestroyAllWindows();
    return 0;
}
