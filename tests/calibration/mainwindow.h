#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "camera.h"
#include "reconstructor.h"
#include "scann.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_bt_abrir_clicked();
    void on_bt_giroHor_clicked();
    void on_bt_transforma_clicked();   
    void on_bt_procesa_clicked();

    void on_bt_exportar_clicked();

private:
    Cam Camara;
    Scanner Escaner;
    Ui::MainWindow *ui;
    cv::Mat image;    

    void muestraImagen();
};

#endif // MAINWINDOW_H
