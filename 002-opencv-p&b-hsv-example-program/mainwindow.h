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

private:
    Ui::MainWindow *ui;
    PDI *pdi;
};

#endif // MAINWINDOW_H
