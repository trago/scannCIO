/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include "CameraCalibrator.h"

// Se abren las imagenes del patron de calibracion y se extraen sus esquinas
int CameraCalibrator::addChessboardPoints(
         const std::vector<std::string>& filelist, 
         cv::Size & boardSize) {

    // Puntos para las esquinas
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    // Escena de Puntos 3D:
    // Inicializa la esquinas in el cuadro de referencia del patron
    // Las esquinas tiene localizacion 3D(X,Y,Z)= (i,j,0)
	for (int i=0; i<boardSize.height; i++) {
		for (int j=0; j<boardSize.width; j++) {

			objectCorners.push_back(cv::Point3f(i, j, 0.0f));
		}
    }

    // Puntos para imagenes 2D:
    cv::Mat image; // Para contener la imagen del patron
    int successes = 0;
    // para todos los puntos de vista
    for (int i=0; i<filelist.size(); i++) {

        // Cargar la imagen
        image = cv::imread(filelist[i],0);

        // Obtener las esquinas
        bool found = cv::findChessboardCorners(
                        image, boardSize, imageCorners);

        // Obtener subpixel de presicion en las esquinas
        cv::cornerSubPix(image, imageCorners, 
                  cv::Size(5,5), 
                  cv::Size(-1,-1), 
			cv::TermCriteria(cv::TermCriteria::MAX_ITER +
                          cv::TermCriteria::EPS, 
             30,		// Numero maximo de interaciones
             0.1));     // Presicion minima

          // Si tenemos un buen tablero, añadirlo a los datos
		  if (imageCorners.size() == boardSize.area()) {

            // Añadir la imagen y los puntos para un punto de vista
            addPoints(imageCorners, objectCorners);
            successes++;
          }

        // Dibujar las esquinas
        cv::drawChessboardCorners(image, boardSize, imageCorners, found);
        cv::imshow("Esquinas en el tablero", image);
        cv::waitKey(100);
    }

	return successes;
}

// Añadir puntos de escena y las imagenes de puntos correspondientes
void CameraCalibrator::addPoints(const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners) {

    // Imagen de puntos para una vista 2D
	imagePoints.push_back(imageCorners);          
    // Puntos de escena correspondiente en 3D
	objectPoints.push_back(objectCorners);
}

// Calibrando la Camara
// Devuelve el error de re-proyeccion
double CameraCalibrator::calibrate(cv::Size &imageSize)
{
    // Destorcionador debe ser reinicializado
	mustInitUndistort= true;

    // Rotaciones y traslaciones de salida
    std::vector<cv::Mat> rvecs, tvecs;

    // Iniciar Calibracion
	return 
     calibrateCamera(objectPoints, // Puntos 3D
                    imagePoints,  // Puntos de la imagen
                    imageSize,    // Tamaño de la imagen
                    cameraMatrix, // Matriz de salida de la camara
                    distCoeffs,   // Matriz de Distorcion de salida
					rvecs, tvecs, // Rs, Ts 
                    flag);        // Opcion de configuracion
//					,CV_CALIB_USE_INTRINSIC_GUESS);

}

// remove distortion in an image (after calibration)
cv::Mat CameraCalibrator::remap(const cv::Mat &image) {

	cv::Mat undistorted;

	if (mustInitUndistort) { // called once per calibration
    
		cv::initUndistortRectifyMap(
			cameraMatrix,  // computed camera matrix
            distCoeffs,    // computed distortion matrix
            cv::Mat(),     // optional rectification (none) 
			cv::Mat(),     // camera matrix to generate undistorted
			cv::Size(640,480),
//            image.size(),  // size of undistorted
            CV_32FC1,      // type of output map
            map1, map2);   // the x and y mapping functions

		mustInitUndistort= false;
	}

	// Apply mapping functions
    cv::remap(image, undistorted, map1, map2, 
		cv::INTER_LINEAR); // interpolation type

	return undistorted;
}


// Set the calibration options
// 8radialCoeffEnabled should be true if 8 radial coefficients are required (5 is default)
// tangentialParamEnabled should be true if tangeantial distortion is present
void CameraCalibrator::setCalibrationFlag(bool radial8CoeffEnabled, bool tangentialParamEnabled) {

    // Set the flag used in cv::calibrateCamera()
    flag = 0;
    if (!tangentialParamEnabled) flag += CV_CALIB_ZERO_TANGENT_DIST;
	if (radial8CoeffEnabled) flag += CV_CALIB_RATIONAL_MODEL;
}

