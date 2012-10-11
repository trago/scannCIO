/*
    Creator: Juan Manuel Ruiz
    Date: October 6th 2012

    This class handles the attributes of the camera and capture method that returns an image.
*/

#ifndef CAM_H
#define CAM_H

#include "scann.h"

class Cam{

public:
    int device;
    cv::Vec2i resolution;

    /**
    Constructor (single)
    @author Juan Manuel Ruiz
    */
    Cam();

    /**
    Constructor with params
    @param[in] number of the device or camera (Index by the system - Greater than 0)
    @param[in] resolution width
    @param[in] resolution height
    @author Juan Manuel Ruiz
    */
    Cam( int dev, int res_width, int res_height );

    /**
    Device Setter
    @param[in] number of the device or camera (Index by the system - Greater than 0)
    @author Juan Manuel Ruiz
    */
    void setDevice( int dev );

    /**
    Resolution Setter
    @param[in] resolution width
    @param[in] resolution height
    @author Juan Manuel Ruiz
    */
    void setResolution(int width, int height);

    /**
    Capture
    Opens a window with a Image stream. User can choose a picture pressing enter key.
    @param[out] image it's the choosen image by the user.
    @author Juan Manuel Ruiz
    */
    bool Capture( cv::Mat& image);

private:

    /**
    Detecta los bordes de la hoja en la imagen

    @param[in] imagen de la cual se extraera la hoja.
    @param[out] borde es el rectangulo que mejor define a la hoja en la imagen.

    @author Juan Manuel Ruiz
   */
    void BordeHoja( cv::Mat& imagen, cv::Rect& borde );

    /**
    Determina cual es el borde de mayor tamaño detectado

    @param[in] bordes es el vector de vectores de puntos.
    @param[out] Valor del indice del vector que contiene el vector de puntos de mayor tamaño.

    @author Juan Manuel Ruiz
    */
    int vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes );

    /**
    Extrae la hoja de la imagen a partir de un borde dado

    @param[out] imagen de la cual se extraera la hoja.

    @author Juan Manuel Ruiz
    */
    void ExtraeHoja( cv::Mat& imagen );

    /**
    Cambia el brillo y contraste de la imagen dada.

    Esta funcion puede trabajar con imagenes RGB o gray. El tipo de datos
    debe ser CV_8U.

    @param[in] image es la imagen a la que le cambiaremos el brillo y contraste
    @param[out] im_res es la imagen que resulta
    @param[in] bright es la cantidad de brillo que se le agrega a la imagen
    @param[in] contrast es la cantidad de contraste que se le agrega a la imagen

    @author Julio C. Estrada
    */
    void changeBrightContrast(cv::Mat image, cv::Mat &im_res, float bright, float contrast);
};

#endif // CAM_H
