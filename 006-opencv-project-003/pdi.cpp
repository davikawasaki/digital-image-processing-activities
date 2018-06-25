// Davi Shinji Mota Kawasaki - 1137280
#include "pdi.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

PDI::PDI()
{

}

PDI::~PDI()
{

}


void PDI::carregarImagem(String filename)
{
    //le a imagem
    imagemRGB = imread(filename,1);
    //caso a imagem tenha sido lida com sucesso
    if(imagemRGB.data)
    {
    //exibe a imagem na janela
    imshow("Imagem Original", imagemRGB);
    }
    //CONVERSAO PARA ESCALA DE CINZA
    cvtColor(imagemRGB,imagemCINZA,COLOR_BGR2GRAY);
    //exibe a imagem cinza na janela
    imshow("Imagem Cinza", imagemCINZA);
    imageLoadedName = filename;
}

void PDI::negativo()
{
     Mat negativo = imagemCINZA.clone();
     for(int x=0;x<negativo.rows;x++)
     {
         for(int y=0;y<negativo.cols;y++)
         {
             uchar pixel = negativo.at<uchar>(x,y);
             negativo.at<uchar>(x,y) = 255-pixel;
         }
     }
     //exibe a imagem na janela
     imshow("Negativo de uma Imagem", negativo);
}

void PDI::ajusteContraste()
{
     Mat contraste = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<contraste.rows;x++)
     {
         for(int y=0;y<contraste.cols;y++)
         {
             uchar pixel = contraste.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }
     //g=((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin
     uchar gmax=255; uchar gmin=0;
     for(int x=0;x<contraste.rows;x++)
     {
         for(int y=0;y<contraste.cols;y++)
         {
             uchar f = contraste.at<uchar>(x,y);
             contraste.at<uchar>(x,y) = ((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin;
         }
     }
     //exibe a imagem na janela
     imshow("Imagem Contraste", contraste);
}

void PDI::histograma()
{
    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    int h[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int nivel = 0;
    for(int x=0;x<imagemCINZA.rows;x++)
    {
        for(int y=0;y<imagemCINZA.cols;y++)
        {
            nivel = (int)imagemCINZA.at<uchar>(x,y);
            h[nivel] += 1;
        }
    }

    //normalizar vetor para plotar o grafico
    int maior=0;
    for(int i=0;i<256;i++)
    {
        if(h[i]>maior) maior=h[i];
    }
    int hn[256];
    for(int i=0;i<256;i++)
    {
        hn[i]=round(h[i]*255/maior);
    }
    //criar imagem em branco para o histrograma
    int altura=256; int largura=512;
    Mat imagemH(altura,largura,CV_8UC3,Scalar(255,255,255));
    Point pt1 = Point(0,0); Point pt2 = Point(0,0);
    for(int i=0; i<altura; i++)
    {
        pt1.x=i*2;pt1.y=altura-1;
        pt2.x=i*2;pt2.y=pt1.y-hn[i];
        line(imagemH,pt1,pt2,Scalar(255,100,50+hn[i]),1,8);
    }

    //exibe a imagem na janela
    imshow("Histograma", imagemH);
}

void PDI::logaritmica()
{
     Mat logImage = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<logImage.rows;x++)
     {
         for(int y=0;y<logImage.cols;y++)
         {
             uchar pixel = logImage.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }
     for(int x=0;x<logImage.rows;x++)
     {
         for(int y=0;y<logImage.cols;y++)
         {
             uchar f = logImage.at<uchar>(x,y);
             double a = 255/log(1+fmax);
             double result = a*log(f+1);
             logImage.at<uchar>(x,y) = (uchar)result;
         }
     }
     //exibe a imagem na janela
     imshow("Imagem Log", logImage);
}

void PDI::potencia()
{
     Mat p = imagemCINZA.clone();
     for(int x=0;x<p.rows;x++)
     {
         for(int y=0;y<p.cols;y++)
         {
             uchar f = p.at<uchar>(x,y);
             double a = 2; double c=1;
             double result = a*pow(f,c);
             p.at<uchar>(x,y) = (uchar)result;
         }
     }
     //exibe a imagem na janela
     imshow("Transf de Potencia ", p);
}

void PDI::equalizacaoHistograma()
{
    Mat eq = imagemCINZA.clone();
    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    int h[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int nivel = 0;
    for(int x=0;x<eq.rows;x++)
    {
        for(int y=0;y<eq.cols;y++)
        {
            nivel = (int)eq.at<uchar>(x,y);
            h[nivel] += 1;
        }
    }

    // Vetor de histograma normalizado
    float hn[256];
    float imgSize = eq.rows * eq.cols;
    for(int i=0;i<256;i++)
    {
        hn[i]=h[i]/imgSize;
    }

    // Vetor de histograma normalizado acumulado
    float hna[256];
    hna[0] = hn[0];
    for(int x=1; x<256; x++) {
        hna[x] = hn[x]+hna[x-1];
    }

    // Cálculo do histograma equalizado
    float hne[256];
    for(int x=0; x<256; x++) {
        hne[x] = round((255)*hna[x]);
    }

    // Troca de valores na imagem final
    int finalNivel = 0;
    for(int x=0;x<eq.rows;x++)
    {
        for(int y=0;y<eq.cols;y++)
        {
            finalNivel = (int)eq.at<uchar>(x,y);
            eq.at<uchar>(x,y) = (int)hne[finalNivel];
        }
    }

    imshow("Imagem Equalizada", eq);

    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    int hst[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) hst[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int hstNivel = 0;
    for(int x=0;x<eq.rows;x++)
    {
        for(int y=0;y<eq.cols;y++)
        {
            hstNivel = (int)eq.at<uchar>(x,y);
            hst[hstNivel] += 1;
        }
    }

    //normalizar vetor para plotar o grafico
    int hstMaior=0;
    for(int i=0;i<256;i++)
    {
        if(hst[i]>hstMaior) hstMaior=hst[i];
    }
    int hstn[256];
    for(int i=0;i<256;i++)
    {
        hstn[i]=round(hst[i]*255/hstMaior);
    }
    //criar imagem em branco para o histrograma
    int altura=256; int largura=512;
    Mat imagemH(altura,largura,CV_8UC3,Scalar(255,255,255));
    Point pt1 = Point(0,0); Point pt2 = Point(0,0);
    for(int i=0; i<altura; i++)
    {
        pt1.x=i*2;pt1.y=altura-1;
        pt2.x=i*2;pt2.y=pt1.y-hstn[i];
        line(imagemH,pt1,pt2,Scalar(255,100,50+hstn[i]),1,8);
    }

    //exibe a imagem na janela
    imshow("Histograma Equalizado", imagemH);
}

void PDI::filtroMedia()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int soma = 0;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) soma += f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) soma += f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) soma += f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else soma += f.at<uchar>(x+i,y+j);
                 }
             }
             g.at<uchar>(x,y)=soma/9;
         }
     }
     //exibe a imagem na janela
     imshow("Filtro da Media", g);
}

void PDI::filtroMediana()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             vector<int> mediana;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) mediana.push_back((int)f.at<uchar>(x-i,y-j));
                     else if(((x+i)<0)||((x+i)>=f.rows)) mediana.push_back((int)f.at<uchar>(x-i,y+j));
                     else if(((y+i)<0)||((y+i)>=f.cols)) mediana.push_back((int)f.at<uchar>(x+i,y-j));
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else mediana.push_back((int)f.at<uchar>(x+i,y+j));
                 }
             }
             sort(mediana.begin(), mediana.end());
             g.at<uchar>(x,y)=mediana.at(4);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro da Mediana", g);
}

void PDI::filtroMinima()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             vector<float> minimo;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) minimo.push_back((float)f.at<uchar>(x-i,y-j));
                     else if(((x+i)<0)||((x+i)>=f.rows)) minimo.push_back((float)f.at<uchar>(x-i,y+j));
                     else if(((y+i)<0)||((y+i)>=f.cols)) minimo.push_back((float)f.at<uchar>(x+i,y-j));
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else minimo.push_back((float)f.at<uchar>(x+i,y+j));
                 }
             }
             sort(minimo.begin(), minimo.end());
             g.at<uchar>(x,y)=minimo.at(0);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Minimo", g);
}

void PDI::filtroMaximo()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             vector<float> maximo;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) maximo.push_back((float)f.at<uchar>(x-i,y-j));
                     else if(((x+i)<0)||((x+i)>=f.rows)) maximo.push_back((float)f.at<uchar>(x-i,y+j));
                     else if(((y+i)<0)||((y+i)>=f.cols)) maximo.push_back((float)f.at<uchar>(x+i,y-j));
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else maximo.push_back((float)f.at<uchar>(x+i,y+j));
                 }
             }
             sort(maximo.begin(), maximo.end());
             g.at<uchar>(x,y)=maximo.at(8);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Maximo", g);
}

void PDI::filtroGaussiana()
{
    // Montando máscara 5x5 com sigma 1
    int maskA = 5, maskB = 5;
    Mat mask = Mat(maskA, maskB, CV_32FC3);
    float sigma = 1;

    // Iterando cada coordenada em cima da função Gaussiana Bidimensional
    int indexA = round((maskA - 1)/2);
    int indexB = round((maskB - 1)/2);
    mask = PDI::_constructMatrix(maskA, indexA, maskB, indexB, sigma, mask);

    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();
    //percorre a imagem inteira
    for(int x=0;x<f.rows;x++)
    {
        for(int y=0;y<f.cols;y++)
        {
            float soma = 0;
            //percorre uma mascara 5x5
            for(int i=-indexA; i<=indexA; i++)
            {
                for(int j=-indexB; j<=indexB; j++)
                {
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) soma += (float)f.at<uchar>(x-i,y-j)*mask.at<float>(i+indexA,j+indexB);
                    else if(((x+i)<0)||((x+i)>=f.rows)) soma += (float)f.at<uchar>(x-i,y+j)*mask.at<float>(i+indexA,j+indexB);
                    else if(((y+i)<0)||((y+i)>=f.cols)) soma += (float)f.at<uchar>(x+i,y-j)*mask.at<float>(i+indexA,j+indexB);
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else soma += (float)f.at<uchar>(x+i,y+j)*mask.at<float>(i+indexA,j+indexB);
                }
            }
            g.at<uchar>(x,y)=soma;
        }
    }
    //exibe a imagem na janela
    imshow("Filtro Gaussiano", g);
}

void PDI::brilhoHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];
    //percorre apenas o plano V
    uchar brilho=0;
    int k=50; //constante para aumentar ou diminuir o brilho
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            brilho = V.at<uchar>(x, y);
            if ((brilho+k) < 0) brilho = 0;
            else if ((brilho+k) > 255) brilho = 255;
            else brilho+=k;
            V.at<uchar>(x, y) = brilho;
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Ajuste de Brilho", imagemSaida);
}

void PDI::equalizacaoHistogramaHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];
    //histograma inicial
    int h[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int nivel = 0;
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            nivel = (int)V.at<uchar>(x,y);
            h[nivel] += 1;
        }
    }

    // Vetor de histograma normalizado
    float hn[256];
    float imgSize = imagemHSV.rows * imagemHSV.cols;
    for(int i=0;i<256;i++)
    {
        hn[i]=h[i]/imgSize;
    }

    // Vetor de histograma normalizado acumulado
    float hna[256];
    hna[0] = hn[0];
    for(int x=1; x<256; x++) {
        hna[x] = hn[x]+hna[x-1];
    }

    // Cálculo do histograma equalizado
    float hne[256];
    for(int x=0; x<256; x++) {
        hne[x] = round((255)*hna[x]);
    }

    // Troca de valores na imagem final
    int finalNivel = 0;
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            finalNivel = (int)V.at<uchar>(x,y);
            V.at<uchar>(x,y) = (int)hne[finalNivel];
        }
    }

    //HISTOGRAMA EQUALIZADO
    int hst[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) hst[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int hstNivel = 0;
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            hstNivel = (int)V.at<uchar>(x,y);
            hst[hstNivel] += 1;
        }
    }

    //normalizar vetor para plotar o grafico
    int hstMaior=0;
    for(int i=0;i<256;i++)
    {
        if(hst[i]>hstMaior) hstMaior=hst[i];
    }
    int hstn[256];
    for(int i=0;i<256;i++)
    {
        hstn[i]=round(hst[i]*255/hstMaior);
    }
    //criar imagem em branco para o histrograma
    int altura=256; int largura=512;
    Mat imagemH(altura,largura,CV_8UC3,Scalar(255,255,255));
    Point pt1 = Point(0,0); Point pt2 = Point(0,0);
    for(int i=0; i<altura; i++)
    {
        pt1.x=i*2;pt1.y=altura-1;
        pt2.x=i*2;pt2.y=pt1.y-hstn[i];
        line(imagemH,pt1,pt2,Scalar(255,100,50+hstn[i]),1,8);
    }

    //exibe a imagem na janela
    imshow("Histograma Equalizado HSV", imagemH);
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Imagem Equalizada", imagemSaida);
}

void PDI::filtroMedianaHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    vector<Mat> planosHSVCopy;
    for(unsigned int v=0; v<planosHSV.size(); v++) {
        planosHSVCopy.push_back(planosHSV[v]);
    }
    for(unsigned int z=0; z<planosHSV.size(); z++)
    {
         //percorre a imagem inteira
         for(int x=0;x<planosHSV[z].rows;x++)
         {
             for(int y=0;y<planosHSV[z].cols;y++)
             {
                 vector<int> mediana;
                 //percorre uma mascara 3x3
                 for(int i=-1; i<2; i++)
                 {
                     for(int j=-1; j<2; j++)
                     {
                         //caso pixels da mascara estejam fora dos limites da imagem
                         //usa-se espelhamento de pixels
                         if(((x+i)<0 && (y+j)<0)||((x+i)>=planosHSV[z].rows && (y+j)>=planosHSV[z].cols)) mediana.push_back((int)planosHSV[z].at<uchar>(x-i,y-j));
                         else if(((x+i)<0)||((x+i)>=planosHSV[z].rows)) mediana.push_back((int)planosHSV[z].at<uchar>(x-i,y+j));
                         else if(((y+i)<0)||((y+i)>=planosHSV[z].cols)) mediana.push_back((int)planosHSV[z].at<uchar>(x+i,y-j));
                         //caso todos os pixels da mascara estejam dentro dos limites da imagem
                         else mediana.push_back((int)planosHSV[z].at<uchar>(x+i,y+j));
                     }
                 }
                 sort(mediana.begin(), mediana.end());
                 planosHSVCopy[z].at<uchar>(x,y)=mediana.at(4);
             }
         }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSVCopy,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro da Mediana", imagemSaida);
}

void PDI::filtroGaussianaHSV()
{
    // Montando máscara 5x5 com sigma 1
    int maskA = 5, maskB = 5;
//    float mask[maskA][maskB];
    Mat mask = Mat(maskA, maskB, CV_32FC3);
    float sigma = 1;

    // Iterando cada coordenada em cima da função Gaussiana Bidimensional
    int indexA = round((maskA - 1)/2);
    int indexB = round((maskB - 1)/2);
    mask = PDI::_constructMatrix(maskA, indexA, maskB, indexB, sigma, mask);

    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    vector<Mat> planosHSVCopy;
    for(unsigned int v=0; v<planosHSV.size(); v++) {
        planosHSVCopy.push_back(planosHSV[v]);
    }

    for(unsigned int z=0; z<planosHSV.size(); z++)
    {
        if (z == 2)
        {
             //percorre a imagem inteira
             for(int x=0;x<planosHSV[z].rows;x++)
             {
                 for(int y=0;y<planosHSV[z].cols;y++)
                 {
                     float soma = 0;
                     //percorre uma mascara 5x5
                     for(int i=-indexA; i<=indexA; i++)
                     {
                         for(int j=-indexB; j<=indexB; j++)
                         {
                             //caso pixels da mascara estejam fora dos limites da imagem
                             //usa-se espelhamento de pixels
                             if(((x+i)<0 && (y+j)<0)||((x+i)>=planosHSV[z].rows && (y+j)>=planosHSV[z].cols)) soma += planosHSV[z].at<uchar>(x-i,y-j)*mask.at<float>(i+indexA,j+indexB);
                             else if(((x+i)<0)||((x+i)>=planosHSV[z].rows)) soma += planosHSV[z].at<uchar>(x-i,y+j)*mask.at<float>(i+indexA,j+indexB);
                             else if(((y+i)<0)||((y+i)>=planosHSV[z].cols)) soma += planosHSV[z].at<uchar>(x+i,y-j)*mask.at<float>(i+indexA,j+indexB);
                             //caso todos os pixels da mascara estejam dentro dos limites da imagem
                             else soma += planosHSV[z].at<uchar>(x+i,y+j)*mask.at<float>(i+indexA,j+indexB);
                         }
                     }
                     planosHSVCopy[z].at<uchar>(x,y)=soma;
                 }
             }
        }
        else
        {
            // Não realiza modificações nos canais H e S
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSVCopy,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro da Gaussiana", imagemSaida);
}

void PDI::limiar()
{
    Mat L = imagemCINZA.clone();
    Mat saida;
    threshold(L,saida,0,255,THRESH_OTSU);

    //exibe a imagem na janela
    imshow("Segmentacao Limiar", saida);
}

void PDI::detectorCanny()
{
    Mat Cinza = imagemCINZA.clone();
    Mat saidaBlur;
    blur(Cinza,saidaBlur,Size(5,5));
    Mat saidaCanny;
    Canny(saidaBlur,saidaCanny,0,255,3);
    //exibe a imagem na janela
    imshow("Detector Canny", saidaCanny);
}

void PDI::detectorSobel()
{
    Mat Cinza = imagemCINZA.clone();
    Mat saidaBlur;
    blur(Cinza,saidaBlur,Size(5,5));
    Mat saidaSobelX,saidaSobelY,saidaSobel;
    saidaSobel = imagemCINZA.clone();
    Sobel(saidaBlur,saidaSobelX,saidaSobel.depth(),1,0,3);
    Sobel(saidaBlur,saidaSobelY,saidaSobel.depth(),0,1,3);

    saidaSobel = saidaSobelX+saidaSobelY;
    //exibe a imagem na janela
    imshow("Detector SobelX", saidaSobelX);
    imshow("Detector SobelY", saidaSobelY);
    imshow("Detector Sobel", saidaSobel);
}

void PDI::transformacoesMorfologicas()
{
    // Cria janela para manipular barras de controle
    namedWindow(morphWindow, CV_WINDOW_AUTOSIZE);

    // Cria barra de controle para escolher operação morfológica (abertura ou fechamento)
    createTrackbar("Operador:\n 0: Abertura - 1: Fechamento",
                   morphWindow,
                   &morphologyOperator,
                   maxMorphologyOperator,
                   &PDI::_onChangeMorphTB,
                   this);

    // Cria barra de controle para selecionar tipo morfológico do kernel
    createTrackbar("Forma:\n 0: Retângulo - 1: Cruz - 2: Elipse",
                   morphWindow,
                   &morphologyKernelType,
                   maxMorphologyKernelType,
                   PDI::_onChangeMorphTB,
                   this);

     // Cria barra de controle para selecionar o tamanho morfológico do kernel
     createTrackbar("Tamanho:\n 2n+1",
                    morphWindow,
                    &morphologyKernelSize,
                    maxMorphologyKernelSize,
                    PDI::_onChangeMorphTB,
                    this);

    PDI::_operacoesMorfologicas(0,0);
}

void PDI::extracaoCaracteristicasBIC(Mat entrada, Mat saida, std::string filename)
{
    unsigned int const coresLimit = 64;
    std::string type = "bic";

    vector<vector<int>> histogramas;
    histogramas.push_back(PDI::_bicAlgorithm(entrada, saida, coresLimit));
    PDI::_extractFeatureHistogram2Txt(true, filename, type, histogramas, coresLimit*2);
}

void PDI::extracaoCaracteristicasCadeia(Mat saidaCINZA, std::string filename)
{
    unsigned int const cadeiaOrientacoesLimit = 8;
    std::string type = "cadeia";

    vector<vector<int>> histogramas;
    histogramas.push_back(PDI::_chainAlgorithmCode(saidaCINZA, 100, cadeiaOrientacoesLimit));
    PDI::_extractFeatureHistogram2Txt(true, filename, type, histogramas, cadeiaOrientacoesLimit);
}

vector<int> PDI::_chainAlgorithmCode(Mat imgCINZA, int limiar, int limit)
{
    // int const limiar = 100;
    // saidaCINZA = imagemCINZA.clone();
    Mat cannySaida;
    vector<vector<Point>> bordas;
    vector<Vec4i> hierarquia;
    // Detecção de borda usando Canny
    Canny(imgCINZA, cannySaida, limiar, limiar*2, 3);
    findContours(cannySaida, bordas, hierarquia, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0,0));

    // Procura maior contorno
    unsigned int maiorIndex = 0;
    unsigned int maiorSize = 0;
    for(unsigned int i=0;i<bordas.size();i++) {
        if(bordas[i].size() > maiorSize) {
            maiorSize = bordas[i].size();
            maiorIndex = i;
        }
    }
    vector<Point> bordaMaior = bordas[maiorIndex];

    // Iteração para obtenção de lista de orientação
    vector<int> cadeiaBorda;

    for(unsigned int i=0;i<bordaMaior.size();i++) {
        if(i+1 < bordaMaior.size()) {
            int direcao = _getDirection(bordaMaior[i], bordaMaior[i+1]);
            cadeiaBorda.push_back(direcao);
        } else {
            int direcao = _getDirection(bordaMaior[i], bordaMaior[0]);
            cadeiaBorda.push_back(direcao);
        }
    }

    // Histograma da cadeia de borda
    vector<int> histCadeia (limit); // zero-initialized ints
    // for(unsigned int i=0;i<cadeiaOrientacoes;i++) {
    //     histCadeia[i] = 0;
    // }
    for(unsigned int i=0;i<cadeiaBorda.size();i++) {
        histCadeia.at(cadeiaBorda[i]) += 1;
    }

    return histCadeia;
}

vector<int> PDI::_bicAlgorithm(Mat imgEntradaRGB, Mat imgSaidaCINZA, int limit)
{
    // 1. Requantização para 64 cores
    // entrada = imagemRGB.clone();
    // saida = imagemCINZA.clone();
    PDI::_reducao64Cores(imgEntradaRGB, imgSaidaCINZA);

    // 2. Obtém pixels de borda e internas
    vector<Point> bordas;
    vector<Point> interiores;
    for(int x=0;x<imgSaidaCINZA.rows;x++)
    {
        for(int y=0;y<imgSaidaCINZA.cols;y++)
        {
            //caso pixels da mascara vizinhança-4 estejam fora dos limites da imagem
            //usa-se_point espelhamento de pixels
            bool borda = false;

            // Vizinhança esquerda
            if(x-1 >= 0) {
                if (imgSaidaCINZA.at<uchar>(x-1,y) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            } else {
                if (imgSaidaCINZA.at<uchar>(x+1,y) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            }
            // Vizinhança direita
            if(x+1 <= imgSaidaCINZA.rows) {
                if (imgSaidaCINZA.at<uchar>(x+1,y) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            } else {
                if (imgSaidaCINZA.at<uchar>(x-1,y) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            }
            // Vizinhança cima
            if(y-1 >= 0) {
                if (imgSaidaCINZA.at<uchar>(x,y-1) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            } else {
                if (imgSaidaCINZA.at<uchar>(x,y+1) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            }
            // Vizinhança baixa
            if(y+1 >= imgSaidaCINZA.cols) {
                if (imgSaidaCINZA.at<uchar>(x,y+1) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            } else {
                if (imgSaidaCINZA.at<uchar>(x,y-1) != imgSaidaCINZA.at<uchar>(x,y)) borda = true;
            }

            if (borda) bordas.push_back(PDI::_makePoint(x, y));
            else interiores.push_back(PDI::_makePoint(x, y));
        }
    }

    // 3. Otenção dos histogramas da borda e do interior
    vector<int> histBordas (limit); // zero-initialized ints
    vector<int> histInteriores (limit); // zero-initialized ints

    // for(int i=0;i<limit;i++) {
    //     histBordas[i] = 0;
    //     histInteriores[i] = 0;
    // }

    int nivel = 0;
    for(unsigned int i=0;i<bordas.size();i++) {
        nivel = (int)imgSaidaCINZA.at<uchar>(bordas[i].x, bordas[i].y);
        histBordas.at(nivel) += 1;
    }
    for(unsigned int i=0;i<bordas.size();i++) {
        Point interior = interiores[i];
        nivel = (int)imgSaidaCINZA.at<uchar>(interior.x, interior.y);
        histInteriores.at(nivel) += 1;
    }

    // 4. Concatena histogramas obtidos
    vector<int> histFinal;
    for(unsigned int i=0;i<histBordas.size();i++) {
        histFinal.push_back(histBordas.at(i));
    }
    for(unsigned int i=0;i<histInteriores.size();i++) {
        histFinal.push_back(histInteriores.at(i));
    }

    // int histFinal[limit*2];
    // copy(histBordas, histBordas + cores, histFinal);
    // copy(histInteriores, histInteriores + cores, histFinal + cores);

    return histFinal;
}

void PDI::extractMultipleFeatures(QStringList fileList)
{
    // vector<Mat> doubleFileImgList;
    // doubleFileImgList = PDI::_loadMultipleFiles(fileList);

    vector<Mat> imgFileRGBList;
    vector<Mat> imgFileCINZAList;
    for (const auto& file : fileList) {
        String filename = file.toStdString();
        Mat imgFileRGB = imread(filename,1);
        if(imgFileRGB.data) {
            imgFileRGBList.push_back(imgFileRGB);
            // Conversão para escala de cinza
            Mat imgFileCINZA;
            cvtColor(imgFileRGB,imgFileCINZA,COLOR_BGR2GRAY);
            // if(imgFileCINZA.data) {
            imgFileCINZAList.push_back(imgFileCINZA);
            // }
        }
    }

    // multipleImgList.clear();
    // multipleImgList = imgFileRGBList;

    bool inputStatus;
    QString imageNameTxt = QInputDialog::getText(0, "Extracao",
                                             "Insira o nome da imagem:", QLineEdit::Normal,
                                             "", &inputStatus);
    if (inputStatus && !imageNameTxt.isEmpty()) {
        inputStatus = false;
        QString extractTypeTxt = QInputDialog::getText(0, "Extracao",
                                                 "Insira o tipo de extracao (bic/cadeia):", QLineEdit::Normal,
                                                 "", &inputStatus);
        if (inputStatus && !extractTypeTxt.isEmpty()) {
            vector<vector<int>> histogramas;
            histogramas.clear();

            if(extractTypeTxt == "bic") {
                unsigned int const coresLimit = 64;
                std::string type = "bic";

                for(unsigned int i=0;i<imgFileRGBList.size();i++) {
                    histogramas.push_back(PDI::_bicAlgorithm(imgFileRGBList.at(i), imgFileCINZAList.at(i), coresLimit));
                }
                PDI::_extractFeatureHistogram2Txt(false, imageNameTxt.toUtf8().constData(), type, histogramas, coresLimit*2);
            } else if(extractTypeTxt == "cadeia") {
                unsigned int const cadeiaOrientacoesLimit = 8;
                std::string type = "cadeia";

                for(unsigned int i=0;i<imgFileCINZAList.size();i++) {
                    histogramas.push_back(PDI::_chainAlgorithmCode(imgFileCINZAList.at(i), 100, cadeiaOrientacoesLimit));
                }
                PDI::_extractFeatureHistogram2Txt(false, imageNameTxt.toUtf8().constData(), type, histogramas, cadeiaOrientacoesLimit);
            } else {
                QMessageBox *msgBox = new QMessageBox;
                msgBox->setWindowTitle("Erro!");
                msgBox->setText("Esse algoritmo de características ainda não foi implementado!");
                msgBox->setStandardButtons(QMessageBox::Ok);
                msgBox->exec();
            }
        } else {
            QMessageBox *msgBox = new QMessageBox;
            msgBox->setWindowTitle("Erro!");
            msgBox->setText("Nenhum algoritmo escolhido!");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->exec();
        }
    } else {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setWindowTitle("Erro!");
        msgBox->setText("Nenhum nome da imagem escolhido!");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }

}

void PDI::_extractFeatureHistogram2Txt(bool singleExtraction, std::string filenameClass, String extractionType, vector<vector<int>> &histograms, unsigned int limit)
{
    // Extração de txt
    String className = filenameClass;
    if(singleExtraction) className = PDI::_getFilename(filenameClass);
    String filename = "algoritmo_" + extractionType + "_imagem_" + className + ".txt";
    ofstream outputFile (filename);
    if (outputFile.is_open()) {
        for(unsigned int i=0;i<limit;i++) {
            outputFile << i << ",";
            if(i == limit-1) outputFile << "class,";
        }
        outputFile << "\n";
        for(unsigned int i=0;i<histograms.size();i++) {
            for(unsigned int j=0;j<histograms.at(i).size();j++) {
                outputFile << histograms[i][j] << ",";
                if(j == histograms.at(i).size()-1) outputFile << className + ",";
            }
            outputFile << "\n";
        }
        outputFile.close();
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setWindowTitle("Sucesso!");
        msgBox->setText("Características extraídas em arquivo txt!");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    } else {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setWindowTitle("Erro!");
        msgBox->setText("Erro na escrita do arquivo de características!");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
}

float PDI::_gaussianaBidimensional(int x, int y, float sigma)
{
    float A = (1/(2*M_PI*pow(sigma, 2)));
    float gaus = A*exp(-(pow(x,2)+pow(y,2))/(2*pow(sigma, 2)));
    return gaus;
}

Mat PDI::_constructMatrix(int maskA, int indexA, int maskB, int indexB, float sigma, Mat mask)
{
    float _soma = 0;
    float value = 0;
    for(int i=-indexA; i<=indexA; i++)
    {
        for(int j=-indexB; j<=indexB; j++)
        {
            value = PDI::_gaussianaBidimensional(i, j, sigma);
            mask.at<float>(i+indexA, j+indexB) = value;
            _soma += value;
        }
    }

    // Obtendo máscara final da gaussiana
    // Iterando cada coordenada obtida pela gaussiana e dividindo pela soma total obtida anteriormente
    for(int i=0; i<maskA; i++)
    {
        for(int j=0; j<maskB; j++)
        {
            mask.at<float>(i,j) = (float)(mask.at<float>(i,j)/_soma);
        }
    }
    return mask;
}

void PDI::_onChangeMorphTB(int value, void* ptr)
{
    PDI* pdi = (PDI*)(ptr);
    pdi->_operacoesMorfologicas(value, ptr);
}

void PDI::_operacoesMorfologicas(int, void*)
{
    Mat imagemMorfologicaEntrada = imagemRGB.clone();

    // Como MORPH_<TIPO> vai de 2 até 6, adiciona-se 2 ao operador escolhido
    int operation = morphologyOperator + 2;

    // Construção e obtenção final do kernel
    Mat kernel = getStructuringElement(morphologyKernelType, Size(2*morphologyKernelSize + 1, 2*morphologyKernelSize+1), Point(morphologyKernelSize, morphologyKernelSize));

    // Aplicação da transformação morfológica com:
    // Imagem de entrada/saída, operação escolhida e kernel usado
    morphologyEx(imagemMorfologicaEntrada, imagemMorfologicaSaida, operation, kernel);
    imshow(morphWindow, imagemMorfologicaSaida);
}

void PDI::_reducao64Cores(Mat &image, Mat &outputImage)
{
    int height = image.rows;                    // number of lines
    int width = image.cols; // number of elements per line
    int step = image.step;
    int stepOutput = outputImage.step;
    uchar* imageData = image.data;
    uchar* outputImageData = outputImage.data;

    for (int i = 0; i < height ; i++)  {
        for (int j = 0; j < width; j++)  {

          // operator XXXXXXXX & 11000000 equivalent to  XXXXXXXX AND 11000000 (=192)
          // operator 01000000 >> 2 is a 2-bit shift to the right = 00010000
          uchar C1 = (imageData[i*step+j*3+0] & 192)>>2;
          uchar C2 = (imageData[i*step+j*3+1] & 192)>>4;
          uchar C3 = (imageData[i*step+j*3+2] & 192)>>6;

          outputImageData[i*stepOutput+j] = C1 | C2 | C3; // merges the 2 MSB of each channel
        }
    }
}

Point PDI::_makePoint(int x, int y) {
    return {x, y};
}

int PDI::_getDirection(Point a, Point b)
{
    // Esquerda ou direita
    if(a.y == b.y) {
        if(a.x > b.x) return 0; // Direita
        else return 4; // Esquerda
    }
    // Cima ou baixo
    else if(a.x == b.x) {
        if(a.y > b.y) return 6; // Baixo
        else return 2; // Cima
    }
    // Direcionamento leste
    else if(a.x > b.x) {
        if(a.y > b.y) return 7; // Nordeste
        else return 1; // Sudeste
    }
    // Direcionamento oeste
    else {
        if(a.y > b.y) return 5; // Sudoeste
        else return 3; // Noroeste
    }
}

String PDI::_getFilename(String fullFilename)
{
    String baseFilename = fullFilename.substr(fullFilename.find_last_of("/\\") + 1);
    size_t p = baseFilename.find_last_of('.');
    return baseFilename.substr(0, p);

    return fullFilename;
}

vector<Mat> PDI::_loadMultipleFiles(QStringList fileList)
{
    vector<Mat> imgFileRGBList;
    vector<Mat> imgFileCINZAList;
    for (const auto& file : fileList) {
        String filename = file.toStdString();
        Mat imgFileRGB = imread(filename,1);
        if(imgFileRGB.data) {
            imgFileRGBList.push_back(imgFileRGB);
            // Conversão para escala de cinza
            Mat imgFileCINZA;
            cvtColor(imgFileRGB,imgFileCINZA,COLOR_BGR2GRAY);
            if(imgFileCINZA.data) {
                imgFileCINZAList.push_back(imgFileCINZA);
            }
        }
    }

    return imgFileRGBList;
}
