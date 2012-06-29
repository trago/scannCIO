#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "CameraCalibrator.h"

using namespace cv;
using namespace std;

void CapturaCamara( int dispositivo );
void CalibraCamara( );

 int main() {

     // Parametros iniciales
     int dispositivo = 0;   // 0 Para la integrada (Default)
     int accion;


     do{
        // Menu
        cout << endl << "Presione \"1\" para calibrar la camara." << endl;
        cout << "Presione \"2\" para escanear." << endl;
        cout << "Elija una accion: ";
        accion = 1;

        cout << accion << endl;

        switch( accion ){
        case 1:
            cout << "Selecciono Calibrar" << endl;
            //CapturaCamara( dispositivo );
            CalibraCamara();
            break;

        case 2:
            cout << "Selecciono escanear" << endl;
            break;
        }


     accion = cvWaitKey() & 255;
     }while( accion != 27 );


     //tecla = cvWaitKey(100) & 255;

    return 0;
 }

 void CapturaCamara( int dispositivo ){
     int tecla;
     int cont = 0;

     // Abrir dispositivo de captura
     //CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY ); // Default (0)
     CvCapture* capture = cvCaptureFromCAM( dispositivo );

    if ( !capture ) {
        fprintf( stderr, "ERROR: Dispositivo de captura %d no encontrado \n", dispositivo );
        getchar();
        return;
    }


    cout << endl << "Calibrando... " << endl;
    cout << "Se capturara una serie de 10 imagenes para calibrar la camara." << endl;
    cout << "Presiona ENTER para capturar una imagen nueva..." << endl;

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
        tecla = cvWaitKey(100) & 255;

        if( tecla == 27 || cont == 10 ){
            // Liberar el dispositivo de captura y la memoria
            cvDestroyWindow( "Video..." );
            cvDestroyWindow( "Imagen Capturada" );
            cvReleaseCapture( &capture );
            return;
        }

        else
            if( tecla == 10 && cont < 10){

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

 }

 void CalibraCamara(){
     // Se crea una ventana con el nombre "Image"
     namedWindow("Imagen");

     // Se crea una imagen vacia
     Mat image;

     // Se crea un vector que almacenara todos los nombres de las imagenes
     vector<string> filelist;

     // ciclo que genera la lista de los nombres de las imagenes
     //for (int i=1; i<=9; i++) {
     for (int i=1; i<=20; i++) {
         // Se genera el nombre para cada imagen incluyendo su ruta
         stringstream str;
         //str << "IMG" << setw(2) << setfill('0') << i << ".png";
         str << "chessboard" << setw(2) << setfill('0') << i << ".jpg";
         cout << str.str() << endl;

         // Se agrega el nombre al vector
         filelist.push_back(str.str());

         // Se muestra cada imagen
         image= imread(str.str(),0);
         imshow("Image",image);

         // Retardo para desplegar las imagenes
         waitKey(100);
     }

     // Se crea un objeto de la clase CameraCalibrator
     CameraCalibrator cameraCalibrator;

     // Se especifica el numero de esquinas verticales y horizontales internas
     Size boardSize(6,4);

     // Se abren todas las imagenes y se extraen sus puntos de las esquinas
     cameraCalibrator.addChessboardPoints(
         filelist,	// Lista de los nombres de las imagenes
         boardSize);	// Numero de esquinas del patron

     // Calibrando la camara
     // cameraCalibrator.setCalibrationFlag(true,true);
     Size size=image.size();
     cameraCalibrator.calibrate(size);

     // Imagen sin distorcion
     //image = cv::imread(filelist[6]);

     //image = cv::imread(filelist[8]);
     //cv::Mat uImage= cameraCalibrator.remap(image);

     // Mostrar la Matriz de la camara
     Mat cameraMatrix= cameraCalibrator.getCameraMatrix();
     cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << endl;
     cout << cameraMatrix.at<double>(0,0) << "\t\t" << cameraMatrix.at<double>(0,1) << "\t\t" << cameraMatrix.at<double>(0,2) << endl;
     cout << cameraMatrix.at<double>(1,0) << "\t\t" << cameraMatrix.at<double>(1,1) << "\t\t" << cameraMatrix.at<double>(1,2) << endl;
     cout << cameraMatrix.at<double>(2,0) << "\t\t" << cameraMatrix.at<double>(2,1) << "\t\t" << cameraMatrix.at<double>(2,2) << endl;

     imshow("Imagen Original", image);
     //imshow("Imagen ajustada, sin distorcion", uImage);
}

