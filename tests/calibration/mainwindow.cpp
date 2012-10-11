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
    //Escaner.setResolution(2592, 1944); // Poner la resolucion de tu camara

    ui->setupUi(this);
    ui->bt_giroHor->setEnabled(false);
    ui->bt_transforma->setEnabled(false);
    ui->bt_procesa->setEnabled(false);
}

// Fucion del Evento Final (Al cerrar el formulario - Destructor)
MainWindow::~MainWindow()
{
    delete ui;
}

// FUNCIONES OPERATIVAS - EVENTOS ==========================================================================================

// Funcion - Evento del Boton "Abrir Imagen"
void MainWindow::on_bt_abrir_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",
                                                           tr("Image Files (*.png *.jpg *.jpeg *.bmp) ") );
    Escaner.GetImage( image, -1, fileName.toAscii().data(), true /*true para que muestre la captura*/);

    if(image.data){
        ui->bt_giroHor->setEnabled(true);
        ui->bt_transforma->setEnabled(true);
        ui->bt_procesa->setEnabled(true);
        muestraImagen();
    }

    else
        if( fileName.toAscii().data() != NULL )
            std::cout << "No se pudo abrir el archivo o el numero de dispositivo no existe" << std::endl;
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
}

// Funcion - Evento del Boton "Procesado"
void MainWindow::on_bt_procesa_clicked()
{

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
