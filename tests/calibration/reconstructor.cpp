#include "reconstructor.h"

// Contructor
reconstructor::reconstructor(cv::Mat &img)
{
    cont = 0;
    img.copyTo(inImg);
    img.copyTo(outImg);
}

// Destructor
reconstructor::~reconstructor()
{
    /*
    cvReleaseImage(inImg);
    cvReleaseImage(outImg);

    if (recImg)
        cvReleaseImage(recImg);
    */
}

// Funcion SET para la captura de los puntos seleccionados
void reconstructor::setPoint(int x, int y){
    // Asignacion de los puntos
    points[cont].x = x;
    points[cont].y = y;

    // Visualizacion de los puntos
    std::cout << "[" << cont << "]" << points[cont].x << ", " << points[cont].y << std::endl;
    cv::circle( inImg, cv::Point( points[cont].x, points[cont].y ), 5, CV_RGB(255,0,0), 3 );

    // Incremetar contador
    cont++;
}

void reconstructor::compute_matrix()
{






















}
