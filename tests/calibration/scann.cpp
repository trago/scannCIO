#include "scann.h"
#include "camera.h"

using namespace std;

// Declaracion de la f uncion que procesa el evento del mouse al hacer clic (Externa a la clase principal)
void mouse_call (int event, int x, int y, int flags, void *param);

Scanner::Scanner(){

}

/* Funciones Principales ================================================================================================ */

// Funcion que para obtener una imagen ya sea de un archivo o de un dispositivo
bool Scanner::GetImage( cv::Mat& imagen, int modo, std::string r_imagen, bool demo){
    if( modo < 0 ){
        if( r_imagen.empty() ){
            return false;
        }
        else {
            imagen = cv::imread(r_imagen);
        }
    }
    else {
        Camara.Capture(imagen);
    }
    if (imagen.empty()) {
        return false;
    }

    m_imagen = imagen;
    return true;
}

// Funcion de prueba que engloba la captura y procesado de la imagen
bool Scanner::Test( cv::Mat& imagen, int modo, std::string r_imagen, bool demo){

    GetImage( imagen, modo, r_imagen, demo );

    // Edicion de imagen
    cv::Mat im2;
    ExtraeHoja(imagen);
    rotarImagen(imagen, 90);
    ProcesaImagen( imagen, im2 );

    im2.copyTo(imagen);
    m_imgscanned = imagen;

    return true;
}

// Funcion que despliega la imagen original y la procesada
void Scanner::showTest()
{
    cv::namedWindow("Original..", CV_WINDOW_NORMAL);
    cv::imshow("Original..", m_imagen);

    cv::namedWindow("Editada..", CV_WINDOW_NORMAL);
    cv::imshow("Editada..", m_imgscanned);

    cv::waitKey();
    cvDestroyAllWindows();
}

/* Operative Methods ==================================================================================================== */

// Funcion para rotar la imagen
void Scanner::rotarImagen( cv::Mat& imagen, double angulo )
{
    if( imagen.rows < imagen.cols ){
        cv::Mat Img( imagen.cols, imagen.rows, imagen.type() );
        cv::Point2f centroImg( imagen.cols / 2.0F, imagen.cols / 2.0F );
        cv::Mat mat_rot  = getRotationMatrix2D (centroImg, angulo, 1.0);
        cv::warpAffine( imagen, Img, mat_rot, Img.size() );
        Img.copyTo(imagen);
    }

    else{
        cv::Mat Img( imagen.cols, imagen.rows, imagen.type() );
        cv::Point2f centroImg( imagen.cols / 2.0F, imagen.cols / 2.0F );
        cv::Mat mat_rot  = getRotationMatrix2D (centroImg, angulo, 1.0);
        cv::warpAffine( imagen, Img, mat_rot, Img.size() );
        Img.copyTo(imagen);
    }
}

// Funcion que detecta la hoja en la imagen
void Scanner::BordeHoja( cv::Mat& imagen, cv::Rect& borde )
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

// Funcion que determina cual es el borde de mayor tamaño detectado
int Scanner::vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes )
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

// Funcion que extrae la hoja de la imagen
void Scanner::ExtraeHoja( cv::Mat& imagen )
{

  cv::Rect borde;
  BordeHoja(imagen, borde);

  // Recortando la imagen al tamaño indicado
  cv::Mat image_roi = imagen(borde);

  image_roi.copyTo(imagen);
}

// Funcion que procesa la imagen.
void Scanner::ProcesaImagen( cv::Mat imagen, cv::Mat &im_res )
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
void Scanner::changeBrightContrast(cv::Mat image, cv::Mat &im_res, float bright, float contrast)
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

// Funcion que realiza la transformacion afin de una imagen
bool Scanner::Transforma( cv::Mat &imagen ){
    cv::Mat im2;
    imagen.copyTo(im2);

    reconstructor rec(im2);

    cv::namedWindow("Seleccion", CV_WINDOW_NORMAL);
    cvSetMouseCallback( "Seleccion", mouse_call, (void*) (&rec) );

    // Tomando los 4 puntos a transformar
    while(rec.cont < 4) {
        if (!rec.inImg.data)
            break;

        cv::imshow("Seleccion", rec.inImg );

        if( cvWaitKey(100) == 27 )
            cv::destroyWindow("Seleccion");
    }

    // Obteniendo Matriz de Transformacion
    cv::Mat TransMat;
    cv::Point2f dst[4];
    dst[0].x= 47;
    dst[0].y = 43;
    dst[1].x = 1157;
    dst[1].y = 43;
    dst[2].x = 47;
    dst[2].y = 1555;
    dst[3].x = 1157;
    dst[3].y = 1555;

    TransMat = cv::getPerspectiveTransform(&rec.points[0], &dst[0]);
    //std::cout << TransMat << std::endl;

    // Realizando la transformación
    cv::Size dsize( 1200, 1600 );
    cv::warpPerspective(imagen, rec.outImg, TransMat, dsize);

    rec.outImg.copyTo(imagen);

    cv::destroyWindow("Seleccion");
    return true;
}

/* FUNCIONES EXTRAS ===================================================================================================== */

// Funcion que procesa el evento del mouse al hacer clic (Externa a la clase principal)
void mouse_call (int event, int x, int y, int flags, void *param)
{
    reconstructor* rec = (reconstructor*) param;

    // Click Izquierdo del Mouse
    if( flags == CV_EVENT_FLAG_LBUTTON )
        rec->setPoint( x, y );
}


