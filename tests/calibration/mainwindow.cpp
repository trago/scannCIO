#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

void mouse_call (int event, int x, int y, int flags, void *param);

// FUNCIONES DEL FORMULARIO ================================================================================================

// Fucion del Evento Principal (Al crear el Formulario - Constructor)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->bt_giroHor->setEnabled(false);
    ui->bt_transforma->setEnabled(false);
    ui->bt_procesa->setEnabled(false);
    ui->bt_exportar->setEnabled(false);

    // Control de apertura de imagen (RadioButtons)
    ui->rb_archivo->setChecked(true);
    ui->rb_dispositivo->setChecked(false);
    ui->cb_dispositivos->setEnabled(false);
    ui->cb_resoluciones->setEnabled(false);

    // Control de los combobox
    llenaCombos();

    //Escaner.Camara.setDevice(0);
    //Escaner.Camara.setResolution(640,480);
    //Escaner.Camara.setResolution(2048,1536);
    //Escaner.Camara.setResolution(2592,1944);
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
        ui->bt_giroHor->setEnabled(true);
        ui->bt_transforma->setEnabled(true);
        ui->bt_procesa->setEnabled(true);
        ui->bt_exportar->setEnabled(true);
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
            cv::imwrite( fileName.toAscii().data(), image);
    }
}

void MainWindow::on_rb_archivo_clicked()
{    
    ui->rb_archivo->setChecked(true);
    controlRadioButtons();
}

void MainWindow::on_rb_dispositivo_clicked()
{
    ui->rb_dispositivo->setChecked(true);
    controlRadioButtons();
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

// Funcion que controla los radiobutton de seleccion "Abrir Imagen"
void MainWindow::controlRadioButtons(){
    if( ui->rb_archivo->isChecked() ){
        ui->rb_dispositivo->setChecked(false);
        ui->cb_dispositivos->setEnabled(false);
        ui->cb_resoluciones->setEnabled(false);

        Escaner.Camara.setDevice(-1);
    }

    if( ui->rb_dispositivo->isChecked() ){
        ui->rb_dispositivo->setChecked(true);
        ui->cb_dispositivos->setEnabled(true);
        ui->cb_resoluciones->setEnabled(true);
    }
}

void MainWindow::llenaCombos(){
    ui->cb_dispositivos->setMaxCount(Escaner.Camara.n_devices);

    std::cout << Escaner.Camara.n_devices << std::endl;

    for(int i=0; i<Escaner.Camara.n_devices; i++){
        std::stringstream str;
        str << i;
        ui->cb_dispositivos->addItem(str.str().c_str());

    }

    int disp = ui->cb_dispositivos->currentText().toInt();
    Escaner.Camara.device = disp;
    llenaComboResolucion(disp);
}

void MainWindow::llenaComboResolucion(int disp ){
    Escaner.Camara.getDeviceInfo(disp);
    // Llenado de combo
    for(int i=0; i<Escaner.Camara.num_resolutions; i++){
        std::stringstream str;
        str << Escaner.Camara.resolutions[i].x << " x " << Escaner.Camara.resolutions[i].y;
        ui->cb_resoluciones->addItem(str.str().c_str());
    }
}

void MainWindow::on_cb_resolucion_currentIndexChanged(const QString &arg1){
    std::cout << ui->cb_dispositivos->currentText().toStdString() << std::endl;
}
