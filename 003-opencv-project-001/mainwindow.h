// Davi Shinji Mota Kawasaki - 1137280
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pdi.h"

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
    void on_actionAbrir_Imagem_triggered();

    void on_actionNegativo_triggered();

    void on_actionEqualizacaoHistograma_triggered();

    void on_actionHistograma_triggered();

    void on_actionAjuste_de_Contraste_triggered();

    void on_actionTransformacao_Logaritmica_triggered();

    void on_actionTransformacao_de_Potencia_triggered();

    void on_actionMedia_triggered();

    void on_actionBrilho_triggered();

    void on_actionMediana_triggered();

    void on_actionMinimo_triggered();

    void on_actionMaximo_triggered();

    void on_actionEqualiza_ao_de_Histograma_triggered();

    void on_actionMediana_2_triggered();

    void on_actionGaussiano_triggered();

    void on_actionGaussiana_triggered();

private:
    Ui::MainWindow *ui;
    PDI *pdi;
};

#endif // MAINWINDOW_H
