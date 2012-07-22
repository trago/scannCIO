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


Mat changeBrightContrast(Mat image, float bright, float contrast)
{
  Mat newImage = Mat::zeros(image.size(), image.type());
  for(int i=0; i<image.rows; i++)
    for(int j=0; j<image.cols; j++)
      for(int c=0; c<3; c++){
        newImage.at<Vec3b>(i,j)[c] =
            saturate_cast<uchar>(contrast*image.at<Vec3b>(i,j)[c] + bright);
      }
  return newImage;
}

int main() {

    // Creamos una instancia de Camara
    Camara WebCam;
    Mat imagen;
    Mat imbin;
    Mat imbin1;
    Mat HSV;

    // Elegimos el Dispositivo con el cual trabajar
    //WebCam.setDispositivo(0);

    // Capturamos la imagen
    //WebCam.Capture( imagen );

    //imwrite("10MPX01.jpg", imagen);
    imagen = imread("10MPX01.jpg");
    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", imagen);

    /*
    // Filtro Canny
    Canny( imagen, imbin, 125, 350 );
    threshold( imagen, imbin1, 128, 255, THRESH_BINARY );
    imshow("Canny", imbin1);
    imwrite( "canny.jpg", imbin1 );
    */

    /*
    // Filto Threshold adaptativo
    int block_size = 91; //Debe ser un valor par <-?? Par o inpar??
    double C = 12.5;
    adaptiveThreshold(imagen, imbin1, 256, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY_INV,
                      block_size, C);

    cvNamedWindow( "Threshold", CV_WINDOW_NORMAL );
    imshow("Threshold", imbin1);
    */    

    // Creando matriz HSV con las dimensiones de la imagen
    HSV.create(imagen.rows, imagen.cols, CV_8U);
    // Convirtiendo la imagen de BGR a HSV
    cvtColor(imagen, HSV, CV_BGR2HSV);

    // Creando una Matriz para cada canal
    Mat H( HSV.size(), CV_8U, 1 );
    Mat S( HSV.size(), CV_8U, 1 );
    Mat V( HSV.size(), CV_8U, 1 );

    // Creando una Matriz para cada canal
    Mat B( HSV.size(), CV_8U, 1 );
    Mat G( HSV.size(), CV_8U, 1 );
    Mat R( HSV.size(), CV_8U, 1 );


    std::vector<Mat > planes;
    planes.push_back(H);
    planes.push_back(S);
    planes.push_back(V);

    split( HSV, planes);

    /*
    namedWindow("H", WINDOW_NORMAL);
    namedWindow("S", WINDOW_NORMAL);
    namedWindow("V", WINDOW_NORMAL);

    imshow( "H", H );
    imshow( "S", S );
    imshow( "V", V );
    */

    //planes.clear();
    //planes.push_back(H);
    //planes.push_back(S);
    //planes.push_back(V);
    //merge(planes, imagen);
    //cvtColor(imagen, imagen, CV_HSV2BGR);
    //namedWindow("Salida", WINDOW_NORMAL);
    //imshow("Salida", imagen);
    //imwrite("10mpx_salida.jpg", imagen);

    Size tam(0,0);
    //GaussianBlur(V, V, tam, 0.5 );
    threshold( V, V, 255, 255, THRESH_TRUNC );
    threshold( V, V, 105, 255, THRESH_BINARY );//

    /*
    // Filto Threshold adaptativo
    int block_size = 91; //Debe ser un valor par <-?? Par o inpar??
    double C = 12.5;
    adaptiveThreshold(imagen, imbin1, 256, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY_INV,
                      block_size, C);
    */

    threshold( H, H, 50, 255, THRESH_BINARY );
    namedWindow("Canal H filtrado", WINDOW_NORMAL);
    namedWindow("Bordes", WINDOW_NORMAL);
    imshow("Canal H filtrado", H);

    // Extrayendo bordes a la capa H binarizada
    vector< vector<Point> > bordes;
    findContours( H, bordes, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    // Dibujando los contornos sobre la capa V
    Mat imborders( H.size(), CV_8U, Scalar(255) ); // Imagen binaria "Blanca" del tamaño de la original
    printf("%d", bordes.size() );

//for( int i = 0; i < bordes.size(); i++ ){
//for( int i = bordes.size()-1; i > 0; i-- ){
    int max = WebCam.vectorMayor(bordes);
    //drawContours( imborders, bordes, max, Scalar(0), 1 );

    drawContours( imborders, bordes, 118, Scalar(0), 1 );


    imshow("Bordes", imborders);

//    waitKey(10);
//}


    // Shapes Descriptors sobre capa V
    //Rect r0 = boundingRect( Mat(bordes[5]) );
    //rectangle( V, r0, Scalar(0), 2 );

    //imwrite("10mpx_filtrada.jpg", V);

    //namedWindow("Filtrada");
    //imshow("Filtrada", V);

    /*
    // Extrayendo bordes
    vector< vector<Point> > bordes;
    findContours( imbin1, bordes, CV_RETR_TREE, CV_CHAIN_APPROX_NONE );
    //imwrite( "ztest.jpg", imborders );
    //printf("%d",bordes.size());
    cvWaitKey();

*/

    waitKey();
    cvDestroyAllWindows();
    return 0;
}
