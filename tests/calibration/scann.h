#ifndef SCANN_H
#define SCANN_H

#include <stdio.h>
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
    Funcion que captura la image de la camara para su procesamiento.

    @param[out] imagen es la imagen capturada.
   */
  bool Capture( cv::Mat& imagen );

  // Funcion que detecta la hoja en la imagen
  void BordeHoja( cv::Mat& imagen, cv::Rect& borde );

  // Funcion que extrae la hoja de la imagen
  void ExtraeHoja( cv::Mat& imagen );

  /**
    Funcion que procesa la imagen.

    La imagen procesada queda en escala de grises.
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

  // Funcion para rotar la imagen +pendiente+
  void rotarImagen( cv::Mat& imagen, double angulo);

  // Funcion que determina cual es el borde de mayor tamaño detectado
  int vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes );

  // Setters
  void setDispositivo( int disp );
  //void setResolucion( int width, int height );

  // Getters
  int getDispositivo();
  cv::Vec2i getResolucion();
private:
  int dispositivo;
  cv::Vec2i resolucion;

};

#endif // SCANN_H

