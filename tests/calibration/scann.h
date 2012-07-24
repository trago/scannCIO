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

  // Funcion que procesa la imagen.
  void ProcesaImagen( cv::Mat imagen, cv::Mat &im_res );

  // Funcion para modicar el brillo y contraste
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

