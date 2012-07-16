#ifndef SCANN_H
#define SCANN_H

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Camara{
    private:
        int dispositivo;

    public:
        // Metodo para capturar la imagen, devuelve una imagen.
        bool Capture( cv::Mat imagen );
        // Metodo para obtener una imagen ya procesada de una web cam.
        //cv::Mat GetImage();

        // Setters
        void setDispositivo( int disp );

        // Getters
        int getDispositivo();
};

#endif // SCANN_H
