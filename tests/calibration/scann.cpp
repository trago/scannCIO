#include "scann.h"

using namespace std;

// Setters
void Camara::setDispositivo( int disp ){
    Camara::dispositivo = disp;
}


// Getters
int Camara::getDispositivo(){
    return Camara::dispositivo;
}


// Funcion
bool Camara::Capture( cv::Mat imagen ){

    int tecla;
    int disp = Camara::getDispositivo();

    // Abrir dispositivo de captura
    CvCapture* capture = cvCaptureFromCAM( disp );

    // Comprobar que existe el dispositivo
    if ( !capture ) {
        //fprintf( stderr, "ERROR: Dispositivo de captura %d no encontrado \n", disp );
        //getchar();
        return false;
    }

    // Crear 2 ventanas para el streaming y la imagen devuelta
    cvNamedWindow( "Stream", CV_WINDOW_AUTOSIZE );

    // Ciclo infinito para stream
    while ( 1 ) {
        // Obtener imagen (frame)
        IplImage* frame = cvQueryFrame( capture );

        // Comprobar
        if ( !frame ) {
            //fprintf( stderr, "ERROR: frame is null...\n" );
            // Liberar el dispositivo de captura y la memoria
            cvDestroyWindow( "Stream" );
            cvReleaseCapture( &capture );
            //getchar();
            return false;
        }

        cvShowImage( "Stream", frame );

        // Al presionar una tecla quitar los bits altos usando el operador AND
        tecla = cvWaitKey(100) & 255;

        if( tecla == 10 ){
            cv::Mat im = frame;
            im.copyTo(imagen);
            imshow("Imagen", imagen);
            cvWaitKey();

            // Liberar el dispositivo de captura y la memoria
            cvDestroyWindow( "Stream" );
            cvReleaseCapture( &capture );

            return true;
       }
    }

}

