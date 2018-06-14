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
        void limiar();
        void detectorCanny();
        void detectorSobel();
        void transformacoesMorfologicas();
        static void _onChangeMorphTB(int, void*);
        void _operacoesMorfologicas(int, void*);
        void extracaoCaracteristicasBIC();
        void _reducaoCores(Mat &image, Mat &outputImage);
        void extracaoCaracteristicasCadeia();
        int _getDirection(Point a, Point b);
        Point _makePoint(int x, int y);
        float _gaussianaBidimensional(int x, int y, float sigma);
        Mat _constructMatrix(int maskA, int indexA, int maskB, int indexB, float sigma, Mat mask);
        String _getFilename(String fullFilename);

        //atributos
        Mat imagemRGB;
        Mat imagemCINZA;
        Mat imagemMorfologicaSaida;
        String imageLoadedName;
        int morphologyOperator = 0;
        int const maxMorphologyOperator = 1;
        int morphologyKernelType = 0;
        int const maxMorphologyKernelType = 2;
        int morphologyKernelSize = 0;
        int const maxMorphologyKernelSize = 21;
        char morphWindow[128] = "Transformacoes Morfologicas";
};

#endif // PDI_H
