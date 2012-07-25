#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
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
    String r_imagen = "240712.jpg";

    // Funcion que obtiene la imagen de la camara
    WebCam.GetImage( imagen, 1 );

    // Funcion que obtiene la imagen de un archivo
    //WebCam.GetImage( imagen, -1, r_imagen );

    return 0;
}
