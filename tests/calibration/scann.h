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
        // Metodo para obtener una imagen ya procesada de una web cam.
        //cv::Mat GetImage();

        // Metodo para capturar la imagen, devuelve una imagen.
        bool Capture( cv::Mat& imagen );

        // Metodo que procesa la imagen.

        // Metodo para rotar la imagen
         void rotarImagen( cv::Mat& imagen, double angulo);

         // Metodo para obtener el Vector borde con mayor tamaño
         int vectorMayor( std::vector< std::vector<cv::Point> >& bordes );


        // Setters
        void setDispositivo( int disp );
        //void setResolucion( int width, int height );

        // Getters
        int getDispositivo();
        cv::Vec2i getResolucion();
};

#endif // SCANN_H
