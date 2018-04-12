#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pdi = new PDI();
}



MainWindow::~MainWindow()
{
    delete ui;
    delete pdi;
}

void MainWindow::on_actionAbrir_Imagem_triggered()
{
    //abre uma caixa de dialogo para selecao de uma imagem
    QString filename = QFileDialog::getOpenFileName(
    this, "Abrir", "C:\\",
    tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)")
    );
    pdi->carregarImagem(filename.toStdString());
}

void MainWindow::on_actionHistograma_triggered()
{
    pdi->histograma();
}

void MainWindow::on_actionNegativo_triggered()
{
    pdi->negativo();
}

void MainWindow::on_actionAjuste_de_Contraste_triggered()
{
    pdi->ajusteContraste();
}

void MainWindow::on_actionTransformacao_Logaritmica_triggered()
{
    pdi->logaritmica();
}

void MainWindow::on_actionTransformacao_de_Potencia_triggered()
{
    pdi->potencia();
}

void MainWindow::on_actionEqualizacaoHistograma_triggered()
{

}
