/*
    Creator: Juan Manuel Ruiz
    Date: October 6th 2012

    This class handles the attributes of the camera and capture method that returns an image.
*/

#ifndef CAM_H
#define CAM_H
#define N_RESOLUTIONS 30

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>


class Cam{

public:
    /** Number of devices available */
    int n_devices;

    /** Struct of devices information */
    struct devices{
        int index_device;
        int num_resolutions;
        cv::Point2f resolutions[N_RESOLUTIONS];
    };

    /** Index of the device in use */
    int device;

    /** Resolution used */
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
    Get information of the conected devices
    @author Juan Manuel Ruiz
    */
    void getDevicesInfo(void);

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
    /** Number of common resolutions values */
    int n_resolutions;

    /** Array with common resolutions values */
    cv::Point2f c_resolutions[N_RESOLUTIONS];

    /** Set the resolutions array with common values
    @author Juan Manuel Ruiz
    */
    void setCommonResolutions();

    /**
    Detect borders in the sheet

    @param[in] image to detect sheet
    @param[out] borde rectangle border to detect a sheet

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
