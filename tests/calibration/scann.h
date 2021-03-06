#ifndef SCANN_H
#define SCANN_H

#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "camera.h"
#include "reconstructor.h"


/**
  Scanner para escanear documentos.
 */
class Scanner{
public:
    Cam Camara;

    /**
    Constructor.
    */
    Scanner();

    /**
    Procesa una imagen, ya sea desde un archivo o desde la camara

    @param[in] numero del dispositivo o camara ( -1 para trabajar con la imagen de archivo )
    @param[in] nombre con ruta del archivo
    @param[out] imagen editada
    @param[in] demo indica si se captura la imagen en modo demostracion.

    @author Juan Manuel Ruiz
    */
    bool Test( cv::Mat& imagen, int modo, std::string r_imagen = "", bool demo=false );

    /**
    Captura imagen, desde un archivo (modo = -1) o desde la camara (modo = 0)

    @param[in] numero del dispositivo o camara ( -1 para trabajar con la imagen de archivo )
    @param[in] nombre con ruta del archivo
    @param[in] imagen a editar

    @author Juan Manuel Ruiz
    */
    bool GetImage( cv::Mat& imagen, int modo, std::string r_imagen = "", bool demo=false );  
    
    /**
     Muestra las imagenes capturadas para demostracion.
     
     @author Julio C. Estrada
     */
    void showTest();

    /**
    Realiza una transformacion afin de una imagen

    @param[out] imagen es la imagen transformada.

    @author Juan Manuel Ruiz
    */
    bool Transforma( cv::Mat& imagen );

    /**
    Procesa la imagen.

    @param[in] imagen a procesar
    @param[out] imagen procesada

    @author Julio C. Estrada
   */
    void ProcesaImagen( cv::Mat imagen, cv::Mat &im_res );

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

    /**
    Rota la imagen a un angulo dado (NO HABILITADA)

    @param[out] imagen a rotar
    @param[in] Angulo en grados al cual se rotara la imagen

    @author Juan Manuel Ruiz
    */
    void rotarImagen( cv::Mat& imagen, double angulo);


private:
    /** Es la imagen capturada de la camara*/
    cv::Mat m_imagen;
    /** es la imagen editada */
    cv::Mat m_imgscanned;

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
};

#endif // SCANN_H

