#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

void mouse_call (int event, int x, int y, int flags, void *param);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Escaner.setResolution(2592, 1944); //Poner la resolucion de tu camara

    ui->setupUi(this);
    ui->bt_giroHor->setEnabled(false);
    ui->bt_transforma->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bt_abrir_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",
                                                           tr("Image Files (*.png *.jpg *.jpeg *.bmp) ") );
    Escaner.setImage( image, -1, fileName.toAscii().data(), true /*true para que muestre la captura*/);

    if(image.data){
        ui->bt_giroHor->setEnabled(true);
        ui->bt_transforma->setEnabled(true);
        muestraImagen();
    }

    else
        if( fileName.toAscii().data() != NULL )
            std::cout << "No se pudo abrir el archivo o el numero de dispositivo no existe" << std::endl;
}

void MainWindow::on_bt_giroHor_clicked()
{
    cv::flip(image,image,1);
    muestraImagen();
}

// Funcion que administra la visualizacion
void MainWindow::muestraImagen(){

    cv::Mat temp;

    // Conversion de tipo de imagen
    if( image.channels() > 1 )
        cvtColor(image, temp, CV_BGR2RGB);
    else
        cvtColor(image, temp, CV_GRAY2RGB);

    float prop;
    if( temp.rows < temp.cols )
        prop = float(temp.cols) / float(temp.rows);
    else
        prop = float(temp.rows) / float(temp.cols);

    std::cout << temp.rows << " "<< temp.cols << " = " << prop << std::endl;
    cv::Size dsize( temp.rows/2*1.33, temp.cols/2*1.33 );
    cv::resize(temp,temp, dsize);

    QImage img = QImage((const unsigned char*) (temp.data), temp.cols, temp.rows, QImage::Format_RGB888);

    // Mostrando en la etiqueta
    ui->lb_imagen->setPixmap(QPixmap::fromImage(img));
    // Reescalando
    ui->lb_imagen->resize(ui->lb_imagen->pixmap()->size());
}

void MainWindow::on_bt_transforma_clicked()
{
   cv::Mat im2;
   image.copyTo(im2);

   reconstructor rec(im2);

    cv::namedWindow("Seleccion", CV_WINDOW_AUTOSIZE);
    cvSetMouseCallback( "Seleccion", mouse_call, (void*) (&rec) );

    while(rec.cont < 4) {
        if (!rec.inImg.data)
            break;

        cv::imshow("Seleccion", rec.inImg );

        if( cvWaitKey(100) == 27 )
            cv::destroyWindow("Seleccion");
    }

    cvDestroyWindow("Seleccion");
}

void mouse_call (int event, int x, int y, int flags, void *param)
{
    reconstructor* rec = (reconstructor*) param;

    // Click Izquierdo del Mouse
    if( flags == CV_EVENT_FLAG_LBUTTON )
        rec->setPoint( x, y );
}

void MainWindow::on_bt_procesa_clicked()
{

}
