#include "camera.h"

/* Constructors ========================================================================================== */

// Constructor (single)
Cam::Cam(){
    setCommonResolutions();
    getNumberDevices();
    devices devs[n_devices];
    getDevicesInfo(devs);
    setDevice(0);
}

// Constructor (with params)
Cam::Cam( int dev, int res_width, int res_height ){
    setCommonResolutions();
    getNumberDevices();
    devices devs[n_devices];
    getDevicesInfo(devs);
    setDevice(dev);
    resolution.x = res_width;
    resolution.y = res_height;
}

/* Setters =============================================================================================== */

void Cam::setDevice(int dev){
    device = dev;
}

void Cam::setResolution(int width, int height){
    resolution.x = width;
    resolution.y = height;
}

/* Operative Methods ===================================================================================== */

void Cam::setCommonResolutions(){

    c_resolutions[0].x = 160;
    c_resolutions[0].y = 120;
    c_resolutions[1].x = 176;
    c_resolutions[1].y = 144;
    c_resolutions[2].x = 320;
    c_resolutions[2].y = 176;
    c_resolutions[3].x = 320;
    c_resolutions[3].y = 240;
    c_resolutions[4].x = 352;
    c_resolutions[4].y = 288;
    c_resolutions[5].x = 432;
    c_resolutions[5].y = 240;
    c_resolutions[6].x = 544;
    c_resolutions[6].y = 288;
    c_resolutions[7].x = 640;
    c_resolutions[7].y = 360;
    c_resolutions[8].x = 640;
    c_resolutions[8].y = 480;
    c_resolutions[9].x = 752;
    c_resolutions[9].y = 416;
    c_resolutions[10].x = 800;
    c_resolutions[10].y = 448;
    c_resolutions[11].x = 800;
    c_resolutions[11].y = 600;
    c_resolutions[12].x = 864;
    c_resolutions[12].y = 480;
    c_resolutions[13].x = 960;
    c_resolutions[13].y = 544;
    c_resolutions[14].x = 960;
    c_resolutions[14].y = 720;
    c_resolutions[15].x = 1024;
    c_resolutions[15].y = 576;
    c_resolutions[16].x = 1184;
    c_resolutions[16].y = 656;
    c_resolutions[17].x = 1280;
    c_resolutions[17].y = 720;
    c_resolutions[18].x = 1280;
    c_resolutions[18].y = 800;
    c_resolutions[19].x = 1280;
    c_resolutions[19].y = 960;
    c_resolutions[20].x = 1280;
    c_resolutions[20].y = 1024;
    c_resolutions[21].x = 1392;
    c_resolutions[21].y = 768;
    c_resolutions[22].x = 1504;
    c_resolutions[22].y = 832;
    c_resolutions[23].x = 1600;
    c_resolutions[23].y = 896;
    c_resolutions[24].x = 1600;
    c_resolutions[24].y = 1200;
    c_resolutions[25].x = 1712;
    c_resolutions[25].y = 960;
    c_resolutions[26].x = 1792;
    c_resolutions[26].y = 1008;
    c_resolutions[27].x = 1920;
    c_resolutions[27].y = 1080;
    c_resolutions[28].x = 2048;
    c_resolutions[28].y = 1536;
    c_resolutions[29].x = 2592;
    c_resolutions[29].y = 1944;

    /*
    // Get the devices resolutions
    for(int i=0; i<N_RESOLUTIONS; i++){
        std::cout << i << ": " << c_resolutions[i].x << ", " << c_resolutions[i].y << std::endl;
    }
    */
}

void Cam::getNumberDevices(){
    // Determine the number of available devices
    CvCapture *cap;
    n_devices = 0;

    while(1){
        cap = cvCreateCameraCapture(n_devices++);
        if (cap == NULL){
            cvReleaseCapture(&cap);
            break;
        }
        cvReleaseCapture(&cap);
    }

    cvReleaseCapture(&cap);
    n_devices--;
}

void Cam::getDeviceInfo(struct devices *devs){

    // Get the resolutions for each device
    for(int i=0; i<n_devices; i++){
        devs[i].index_device = i;
        devs[i].num_resolutions = 0;
        CvCapture *cap = cvCreateCameraCapture(i);
        for(int j=0; j<N_RESOLUTIONS; j++){
            // Set actual values of resolution
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH, c_resolutions[j].x );
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT, c_resolutions[j].y );

            // Compare the actual resolution value with the last accepted value (Width and Height)
            if( c_resolutions[j].x == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH)
                && c_resolutions[j].y == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT) ){
                devs[i].resolutions[devs[i].num_resolutions].x = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH);
                devs[i].resolutions[devs[i].num_resolutions].y = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT);
                devs[i].num_resolutions++;
            }
        }
        cvReleaseCapture(&cap);
    }
}

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
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, resolution.x );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, resolution.y );
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
