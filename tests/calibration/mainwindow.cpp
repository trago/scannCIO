#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

#include "reconstructor.h"

void mouse_call (int event, int x, int y, int flags, void *param);

// FUNCIONES DEL FORMULARIO ================================================================================================

// Fucion del Evento Principal (Al crear el Formulario - Constructor)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Escaner.Camara.setDevice(1);
    //Escaner.Camara.setResolution(640,480);
    Escaner.Camara.setResolution(2592,1944);

    ui->setupUi(this);
    ui->bt_giroHor->setEnabled(false);
    ui->bt_transforma->setEnabled(false);
    ui->bt_procesa->setEnabled(false);
}

// Fucion del Evento Final (Al cerrar el formulario - Destructor)
MainWindow::~MainWindow()
{
    delete ui;
    cvDestroyAllWindows();
}

// FUNCIONES OPERATIVAS - EVENTOS ==========================================================================================

// Funcion - Evento del Boton "Abrir Imagen"
void MainWindow::on_bt_abrir_clicked()
{
    if( Escaner.Camara.device < 0 ){
        QString fileName = QFileDialog::getOpenFileName( this, tr("Abrir Imagen"), ".",
                                                           tr("Image Files (*.png *.jpg *.jpeg *.bmp) ") );

        if( fileName.toAscii().data() == NULL )
            std::cout << "No se pudo abrir el archivo o el numero de dispositivo no existe" << std::endl;
        else
            Escaner.GetImage( image, Escaner.Camara.device, fileName.toAscii().data(), true /*true para que muestre la captura*/);
    }
    else{
        Escaner.GetImage( image, Escaner.Camara.device );
    }

    if(image.data){
        //ui->bt_giroHor->setEnabled(true);
        ui->bt_transforma->setEnabled(true);
        ui->bt_procesa->setEnabled(true);
        muestraImagen();
    }
}

// Funcion - Evento del Boton "Giro Horizontal"
void MainWindow::on_bt_giroHor_clicked()
{
    cv::flip(image,image,1);
    muestraImagen();
}

// Funcion - Evento del Boton "Transforma"
void MainWindow::on_bt_transforma_clicked()
{
   Escaner.Transforma(image);
   muestraImagen();
}

// Funcion - Evento del Boton "Procesado"
void MainWindow::on_bt_procesa_clicked()
{
    cv::Mat imres;
    Escaner.ProcesaImagen(image,imres);
    imres.copyTo(image);
    muestraImagen();
}

void MainWindow::on_bt_exportar_clicked()
{
    if( image.data ){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Exportar Imagen"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)") );

        if( fileName.toAscii().data() != NULL )
            cv::imwrite( fileName.toAscii().data(),image);
    }
}

// FUNCIONES EXTRAS ========================================================================================================

// Funcion que administra la visualizacion en la GUI
void MainWindow::muestraImagen(){

    cv::Mat temp;

    // Conversion de tipo de imagen
    if( image.channels() > 1 )
        cvtColor(image, temp, CV_BGR2RGB);
    else
        cvtColor(image, temp, CV_GRAY2RGB);

    if( temp.rows > temp.cols )
        Escaner.rotarImagen(temp,90);

    cv::Size dsize( 900, 675);
    cv::resize(temp,temp, dsize);

    QImage img = QImage((const unsigned char*) (temp.data), temp.cols, temp.rows, QImage::Format_RGB888);

    // Mostrando en la etiqueta
    ui->lb_imagen->setPixmap(QPixmap::fromImage(img));

    // Reescalando
    ui->lb_imagen->resize(ui->lb_imagen->pixmap()->size() );
}

