#ifndef SCANN_H
#define SCANN_H

#include <stdio.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Camara{
    private:
        int dispositivo;
        cv::Vec2i resolucion;

    public:

        // Funcion que captura la image de la camara para su procesamiento
        bool Capture( cv::Mat& imagen );

        // Funcion que detecta la hoja en la imagen
        void DetectaHoja( cv::Mat& imagen );

        // Funcion que extrae la hoja de la imagen
        void ExtraeHoja( cv::Mat& imagen );

        // Funcion que procesa la imagen.

        // Funcion para rotar la imagen
         void rotarImagen( cv::Mat& imagen, double angulo);

         // Funcion que determina cual es el borde de mayor tamaño detectado
         int vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes );

        // Setters
        void setDispositivo( int disp );
        //void setResolucion( int width, int height );

        // Getters
        int getDispositivo();
        cv::Vec2i getResolucion();
};

#endif // SCANN_H
