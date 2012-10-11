#include "camera.h"

/* Constructors ========================================================================================== */

// Constructor (single)
Cam::Cam(){
    device = 0;
}

// Constructor (with params)
Cam::Cam( int dev, int res_width, int res_height ){
    device = dev;
    resolution[0] = res_width;
    resolution[1] = res_height;
}

/* Setters =============================================================================================== */

void Cam::setDevice(int dev){
    device = dev;
}

void Cam::setResolution(int width, int height){
    resolution[0] = width;
    resolution[1] = height;
}

/* Operative Methods ===================================================================================== */

bool Cam::Capture( cv::Mat& imagen ){
    int keypress;

    // Opens the capture device.
    // If well use "CvCapture* capture = cvCaptureFromCAM( disp );" works fine,
    // it can't handles the camera params.
    // We prefer use:  cvCreateCameraCapture()
    CvCapture* capture = cvCreateCameraCapture( device );

    // Check Device
    if ( !capture ) {
        /*
        fprintf( stderr, "ERROR: Unknown Capture Device  \n", device );
        getchar();
        */
        return false;
    }

    // Set Resolution (For the 10Mpx Camera use 2592x1944)
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, resolution[0] );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, resolution[1] );
    // For other camera manipulable parameters check documentation: cvSetCaptureProperty

    // Stream Window
    cvNamedWindow( "Press enter to capture...", CV_WINDOW_NORMAL);//  CV_WINDOW_AUTOSIZE);

    // Infinite cicle for stream
    while ( 1 ) {
        // Get frame
        IplImage* frame = cvQueryFrame( capture );

        // Check frame
        if ( !frame ) {
            // fprintf( stderr, "ERROR: frame is null...\n" );
            // Release capture device and memory
            cvDestroyAllWindows();
            cvReleaseCapture( &capture );
            //getchar();
            return false;
        }

        cv::Mat im = frame;

        // Detecting borders in the image
        cv::Rect borde;
        BordeHoja(im, borde);
        rectangle( im, borde, cv::Scalar(255), 2 );

        // Shows image in a window
        cvShowImage( "Press enter to capture...", frame );

        // Eliminate High bits in keypress with AND operator
        keypress = cvWaitKey(100) & 255;

        // Until press Enter key...
        if( keypress == 10 ){
            // Copy the frame in the in image
            im = cvQueryFrame( capture );
            im.copyTo(imagen);
            break;
        }
    }

    // Release capture device and memory
    cvDestroyAllWindows();
    cvReleaseCapture( &capture );
    return true;
}

// Funcion que detecta la hoja en la imagen
void Cam::BordeHoja( cv::Mat& imagen, cv::Rect& borde )
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
  std::vector< std::vector<cv::Point> > bordes;
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

int Cam::vectorMayor( std::vector< std::vector<cv::Point_<int> > >& bordes ){
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

// Funcion para modicar el brillo y contraste
void Cam::changeBrightContrast(cv::Mat image, cv::Mat &im_res, float bright, float contrast)
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
