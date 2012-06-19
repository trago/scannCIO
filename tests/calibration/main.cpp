#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

// A Simple Camera Capture Framework
 int main() {

     int dispositivo = 1;   // 0 Para la integrada (Default)
     int cont = 0;      // Contador para la captura de una imagen

     // Abrir dispositivo de captura
     //CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY ); // Default (0)
     CvCapture* capture = cvCaptureFromCAM( dispositivo );

    if ( !capture ) {
        fprintf( stderr, "ERROR: Dispositivo de captura %d no encontrado \n", dispositivo );
        getchar();
        return -1;
    }

    cout << "Presiona ENTER para capturar una imagen" << endl;

    // Se crean 2 windows donde se mostara el streaming y una imagen que se guardara
    cvNamedWindow( "Video...", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "Imagen Capturada", CV_WINDOW_AUTOSIZE );

    // Ciclo infinito para obtener las imagenes de la camara
    while ( 1 ) {
        // Obtener una imagen (frame)
        IplImage* frame = cvQueryFrame( capture );

        // En caso de no obtener nada enviar mensaje y cerrar ciclo
        if ( !frame ) {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }

        cvShowImage( "Video...", frame );

        // No borrar el objeto frame
        // Al presionar la tecla ESC (0x10001B) en OpenCV 0.9.7(linux),
        // quitar los bits altos usando el operador AND
        int tecla = cvWaitKey(100) & 255;

        if( tecla == 27 )
            break;

        else
            if( tecla == 10 ){

                stringstream cad;
                string cadena;

                cad << "IMG" << setw(2) << setfill('0') << cont << ".png";
                cadena = cad.str();

                Mat mat = frame;

                imwrite( cadena, mat );
                cvShowImage( "Imagen Capturada", frame );
                cout << "Imagen: " << cadena << " Capturada ... " << endl;
                cont++;
            }
    }

    // Liberar el dispositivo de captura y la memoria
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Video..." );
    cvDestroyWindow( "Imagen Capturada" );

    return 0;
 }
