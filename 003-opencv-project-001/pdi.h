// Davi Shinji Mota Kawasaki - 1137280
#ifndef PDI_H
#define PDI_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


class PDI
{
    public:
        //metodos
        PDI();
        ~PDI();
        void carregarImagem(String filename);
        void negativo();
        void logaritmica();
        void potencia();
        void ajusteContraste();
        void histograma();
        void equalizacaoHistograma();
        void filtroMedia();
        void filtroMediana();
        void filtroMinima();
        void filtroMaximo();
        void filtroGaussiana();
        void brilhoHSV();
        void equalizacaoHistogramaHSV();
        void filtroMedianaHSV();
        void filtroGaussianaHSV();
        float _gaussianaBidimensional(int x, int y, float sigma);
        Mat _constructMatrix(int maskA, int indexA, int maskB, int indexB, float sigma, Mat mask);

        //atributos
        Mat imagemRGB;
        Mat imagemCINZA;
};

#endif // PDI_H
