#include "scann.h"

using namespace std;

Camara::Camara()
{
  dispositivo = 0;
}

// Setters
void Camara::setDispositivo( int disp ){
  dispositivo = disp;
}

/*
void Camara::setResolucion(int width, int height){
    Camara.resolucion(0) = width;
    Camara.resolucion(1) = height;
}
*/

// Getters
int Camara::getDispositivo(){
    return dispositivo;
}

/*
cv::Vec2i Camara::getResolucion(){
    return Camara.resolucion;
}
*/

void Camara::rotarImagen( cv::Mat& imagen, double angulo )
{
    /*
    cv::Mat Img( imagen.cols, imagen.rows, imagen.type() );

    printf("%d x %d => %d x %d", imagen.size().height, imagen.size().width, Img.size().height, Img.size().width );

    cv::Point2f centroImg( imagen.cols / 2.0F, imagen.rows / 2.0F );
    cv::Mat mat_rot  = getRotationMatrix2D (centroImg, angulo, 1.0);

    cv::warpAffine ( imagen, Img, mat_rot, Img.size() );

    //cvNamedWindow("Rotada", CV_WINDOW_NORMAL);
    //cv::imshow("Rotada", Img);
    Img.copyTo(imagen);
    */
}


// Funcion que determina cual es el borde de mayor tamaño detectado
int Camara::vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes )
{
  int M = bordes.size(), N;
  int max = bordes[0].size();;
  int idx = 0;
  for( int i = 1; i<M; i++ ){
    N = bordes[i].size();
    if( N > max ){
      max = N;
      idx = i;
    }
  }
  return idx;
}

// Funcion que captura la image de la camara para su procesamiento
bool Camara::Capture( cv::Mat& imagen )
{

  int tecla;
  int disp = getDispositivo();

  // Abrir dispositivo de captura, se puede usar la linea siguiente pero con ella
  // no podemos manipular los parametros de la camara por eso se usa cvCreateCameraCapture()
  // CvCapture* capture = cvCaptureFromCAM( disp );
  CvCapture* capture = cvCreateCameraCapture( disp );

  // Comprobar que existe el dispositivo
  if ( !capture ) {
    //fprintf( stderr, "ERROR: Dispositivo de captura %d no encontrado \n", disp );
    //getchar();
    return false;
  }

  // Elegir Resolucion (1600 x 1200)
//  cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1600 );
//  cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 1200 );
  cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 2592 );
  cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 1944 );


  /*
    // Otro Parametros
    cvSetCaptureProperty( capture, CV_CAP_PROP_SATURATION, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_BRIGHTNESS, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_CONTRAST, 0.7 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_HUE, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_GAIN, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_EXPOSURE, 0.7 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_CONVERT_RGB, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_RECTIFICATION, 0.5 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_GAIN, 0.5 );
    */

  // Crear ventana para el streaming
  cvNamedWindow( "Press enter to capture...", CV_WINDOW_NORMAL);//  CV_WINDOW_AUTOSIZE);

  // Ciclo infinito para stream
  while ( 1 ) {
    // Obtener imagen (frame)
    IplImage* frame = cvQueryFrame( capture );

    // Comprobar si se obtiene o no la imagen
    if ( !frame ) {
      //fprintf( stderr, "ERROR: frame is null...\n" );
      // Liberar el dispositivo de captura y la memoria
      cvDestroyAllWindows();
      cvReleaseCapture( &capture );
      //getchar();
      return false;
    }

    // Falta rotar la imagen... OPCIONAL

    // Se detecta la imagen frente a la camara
    cv::Rect borde;
    cv::Mat im = frame;
    BordeHoja(im, borde);
    rectangle( im, borde, cv::Scalar(255), 2 );

    // Se muestra la imagen en la ventana
    cvShowImage( "Press enter to capture...", frame );

    // Al presionar una tecla quitar los bits altos usando el operador AND
    tecla = cvWaitKey(100) & 255;

    // Si se presiona la tecla enter...
    if( tecla == 10 ){
      // Guardar en la imagen pasada como parametro
      im = cvQueryFrame( capture );
      im.copyTo(imagen);

      // Liberar el dispositivo de captura y la memoria
      cvDestroyAllWindows();
      cvReleaseCapture( &capture );
      return true;
    }
  }

}

// Funcion que detecta la hoja en la imagen
void Camara::BordeHoja( cv::Mat& imagen, cv::Rect& borde )
{

  // Creando matriz HSV con las dimensiones de la imagen
  cv::Mat HSV;
  cv::Mat temp;

  HSV.create(imagen.rows, imagen.cols, CV_8U);

  // Convirtiendo la imagen de BGR a HSV
  cvtColor(imagen, HSV, CV_BGR2HSV);

  // Creando una Matriz para cada canal
  cv::Mat H( HSV.size(), CV_8U, 1 );
  cv::Mat S( HSV.size(), CV_8U, 1 );
  cv::Mat V( HSV.size(), CV_8U, 1 );

  // Creando un vector con los canales
  std::vector<cv::Mat> planes;
  planes.push_back(H);
  planes.push_back(S);
  planes.push_back(V);

  // Separando los canales
  split( HSV, planes);

  // Binarizando
  H.copyTo(temp);
  changeBrightContrast(H, H, 15, 3);
  //cv::threshold( H, H, 1, 255, cv::THRESH_BINARY );

  // Filto Threshold adaptativo
  int block_size = 31; //Debe ser un valor par <-?? Par o inpar??
  double C = 2.5;//12.5;
  cv::adaptiveThreshold(H, H, 256, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, block_size, C);


  // Extrayendo bordes a la capa H binarizada
  vector< vector<cv::Point> > bordes;
  findContours( H, bordes, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

  // Dibujando los contornos sobre la capa H
  cv::Mat imborders( H.size(), CV_8U, cv::Scalar(255) ); // Imagen binaria "Blanca" del tamaño de la original

  int max = vectorMayor(bordes);
  drawContours( imborders, bordes, max, cv::Scalar(0), 1 );

  //cvNamedWindow("Bordes", CV_WINDOW_NORMAL);
  //cv::imshow("Bordes", imborders);

  // Shapes Descriptors sobre capa H
  borde = cv::boundingRect( cv::Mat(bordes[max]) );
  //rectangle( imborders, borde, Scalar(0), 2 );
}

// Funcion que extrae la hoja de la imagen
void Camara::ExtraeHoja( cv::Mat& imagen )
{

  cv::Rect borde;
  BordeHoja(imagen, borde);

  // Recortando la imagen al tamaño indicado
  cv::Mat image_roi = imagen(borde);

  image_roi.copyTo(imagen);
}

// Funcion que procesa la imagen.
void Camara::ProcesaImagen( cv::Mat imagen, cv::Mat &im_res )
{
  /*
    // Filtro Canny
    Canny( imagen, imbin, 125, 350 );
    threshold( imagen, imbin1, 128, 255, THRESH_BINARY );
    imshow("Canny", imbin1);
    imwrite( "canny.jpg", imbin1 );
    */

  // Filto Threshold adaptativo
  //int block_size = 91; //Debe ser un valor par <-?? Par o inpar??
  //double C = 12.5;
  //cv::adaptiveThreshold(imagen, im2, 256, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, block_size, C);
  //cv::threshold( imagen, im_res, 128, 255, cv::THRESH_BINARY );

  cv::Mat aux_imagen;
  cv::cvtColor(imagen, aux_imagen, CV_BGR2GRAY);
  cv::cvtColor(imagen, im_res, CV_BGR2GRAY);
  cv::GaussianBlur(aux_imagen, aux_imagen, cv::Size(151,151), 0);
  //cv::GaussianBlur(im_res, im_res, cv::Size(3,3), 0);
  aux_imagen = im_res - aux_imagen;
  //cv::normalize(aux_imagen, aux_imagen, 0, 255, cv::NORM_MINMAX);
  cv::equalizeHist(aux_imagen, aux_imagen);
  changeBrightContrast(aux_imagen, im_res, 20, 4.);
}

// Funcion para modicar el brillo y contraste
void Camara::changeBrightContrast(cv::Mat image, cv::Mat &im_res, float bright, float contrast)
{
  cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());
  if(image.channels() == 3)
    for(int i=0; i<image.rows; i++)
      for(int j=0; j<image.cols; j++)
        for(int c=0; c<3; c++){
          newImage.at<cv::Vec3b>(i,j)[c] =
              cv::saturate_cast<uchar>(contrast*image.at<cv::Vec3b>(i,j)[c] +
                                       bright);
        }
  else if(image.channels() == 1)
    for(int i=0; i<image.rows; i++)
      for(int j=0; j<image.cols; j++)
          newImage.at<uchar>(i,j) =
              cv::saturate_cast<uchar>(contrast*image.at<uchar>(i,j) +
                                       bright);

  newImage.copyTo(im_res);
}


bool Camara::GetImage( cv::Mat& imagen, int modo, std::string r_imagen ){

    if( modo < 0 ){

        if( r_imagen.empty() ){
            return false;
        }
        else {
            imagen = cv::imread(r_imagen);
        }
    }
    else {
        setDispositivo( modo );
        Capture(imagen);
    }

    cv::namedWindow("Original..", CV_WINDOW_NORMAL);
    cv::imshow("Original..", imagen);

    // Edicion de imagen
    cv::Mat im2;
    ExtraeHoja(imagen);
    rotarImagen(imagen, 90);
    ProcesaImagen( imagen, im2 );

    im2.copyTo(imagen);

    cv::namedWindow("Editada..", CV_WINDOW_NORMAL);
    cv::imshow("Editada..", im2);

    cv::waitKey();
    cvDestroyAllWindows();
    return true;
}
