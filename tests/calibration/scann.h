#ifndef SCANN_H
#define SCANN_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


/**
  Camara para escanear documentos.
 */
class Camara{
public:
    /**
    Constructor.
    */
    Camara();

    /**
    Captura y procesa una imagen, ya sea desde un archivo o desde la camara

    @param[in] numero del dispositivo o camara ( -1 para trabajar con la imagen de archivo )
    @param[in] nombre con ruta del archivo
    @param[out] imagen editada

    @author Juan Manuel Ruiz
    */
    bool GetImage( cv::Mat& imagen, int modo, std::string r_imagen = "" );

    // Setters
    void setDispositivo( int disp );
    //void setResolucion( int width, int height );

    // Getters
    int getDispositivo();
    //cv::Vec2i getResolucion();

private:

    int dispositivo;
    cv::Vec2i resolucion;

    /**
    Captura una image de la camara para su procesamiento.

    @param[out] imagen es la imagen capturada.

    @author Juan Manuel Ruiz
    */
    bool Capture( cv::Mat& imagen );

    /**
    Detecta los bordes de la hoja en la imagen

    @param[in] imagen de la cual se extraera la hoja.
    @param[out] borde es el rectangulo que mejor define a la hoja en la imagen.

    @author Juan Manuel Ruiz
   */
    void BordeHoja( cv::Mat& imagen, cv::Rect& borde );

    /**
    Extrae la hoja de la imagen a partir de un borde dado

    @param[out] imagen de la cual se extraera la hoja.

    @author Juan Manuel Ruiz
    */
    void ExtraeHoja( cv::Mat& imagen );

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

    /**
    Determina cual es el borde de mayor tamaño detectado

    @param[in] bordes es el vector de vectores de puntos.
    @param[out] Valor del indice del vector que contiene el vector de puntos de mayor tamaño.

    @author Juan Manuel Ruiz
    */
    int vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes );

};

#endif // SCANN_H

