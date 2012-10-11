#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "scann.h"
#include "camera.h"

using namespace cv;
using namespace std;

int main() {

    Cam WebCam;
    // Params by device
    WebCam.setDevice(1);
    WebCam.setResolution(640,480);

    // Capture fuction
    Mat image;
    WebCam.Capture(image);

    namedWindow("Imagen obtenida", WINDOW_NORMAL);
    imshow("Imagen obtenida", image);

/*
    // Creamos una instancia de Camara
    Camara WebCam;
    WebCam.setResolution(2592, 1944); //Poner la resolucion de tu camara

    Mat imagen;
    String r_imagen = "240712.jpg";

    // Funcion que obtiene la imagen de la camara
    //WebCam.GetImage( imagen, 0, "", false /*true para que muestre la captura*);

    WebCam.Test( imagen, -1, r_imagen, true /*true para que muestre la captura*);

    //imagen = imread("10MPX01.jpg");
    if (imagen.empty()) {
        cout << "No se pudo abrir el archivo o el numero de dispositivo no existe"<<endl;
        return 1;
    }
    
    WebCam.showTest();
    //namedWindow("Obtenida de la camara..", WINDOW_NORMAL);
    //imshow("Obtenida de la camara..", imagen);
*/
    cvWaitKey();
    return 0;
}
