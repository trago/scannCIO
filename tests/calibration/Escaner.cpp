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
    WebCam.setDispositivo(1);

    // Capturamos la imagen
    WebCam.Capture( imagen );
    imshow("Imagen", imagen);


    return 0;
}
