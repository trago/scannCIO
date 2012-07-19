#include "scann.h"

using namespace std;

// Setters
void Camara::setDispositivo( int disp ){
    Camara::dispositivo = disp;
}

/*
void Camara::setResolucion(int width, int height){
    Camara.resolucion(0) = width;
    Camara.resolucion(1) = height;
}
*/

// Getters
int Camara::getDispositivo(){
    return Camara::dispositivo;
}

/*
cv::Vec2i Camara::getResolucion(){
    return Camara.resolucion;
}
*/

 void rotarImagen( cv::Mat& imagen, double angulo) {
     cv::Point2f centroImg( imagen.cols / 2.0F, imagen.rows / 2.0F);
     cv::Mat mat_rot  = getRotationMatrix2D (centroImg, angulo, 1.0);
     cv::Mat temp;
     cv::warpAffine ( imagen, temp, mat_rot, imagen.size() );
     temp.copyTo(imagen);
 }

// Funcion
bool Camara::Capture( cv::Mat& imagen ){

    int tecla;
    int disp = Camara::getDispositivo();

    // Abrir dispositivo de captura
    //CvCapture* capture = cvCaptureFromCAM( disp );

    CvCapture* capture = cvCreateCameraCapture( disp );


    // Comprobar que existe el dispositivo
    if ( !capture ) {
        //fprintf( stderr, "ERROR: Dispositivo de captura %d no encontrado \n", disp );
        //getchar();
        return false;
    }

    // Elegir Resolucion
    //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1280 );
    //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 1024 );

    // Resolucion (3264×2448 8Mpx)
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1600 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 1200 );

    //cvSetCaptureProperty( capture, CV_CAP_PROP_SATURATION, 0.5 );
    //cvSetCaptureProperty( capture, CV_CAP_PROP_BRIGHTNESS, 0.5 );
    //cvSetCaptureProperty( capture, CV_CAP_PROP_CONTRAST, 0.7 );
   /*

    cvSetCaptureProperty( capture, CV_CAP_PROP_HUE, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_GAIN, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_EXPOSURE, 0.7 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_CONVERT_RGB, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_RECTIFICATION, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_GAIN, 0.5 );
    */
    //printf("%f", cvGetCaptureProperty(capture, CV_CAP_PROP_GAIN) );

    //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 2448 );


    // Crear 2 ventanas para el streaming y la imagen devuelta
    cvNamedWindow( "Stream", CV_WINDOW_NORMAL);//  CV_WINDOW_AUTOSIZE);

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
            //imshow("Imagen", imagen);
            cv::imwrite("pruebaHD2.jpg",imagen);
            //cvWaitKey();

            // Liberar el dispositivo de captura y la memoria
            cvDestroyAllWindows();
            cvReleaseCapture( &capture );

            return true;
       }
    }

}

