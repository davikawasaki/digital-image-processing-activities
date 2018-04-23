#include "pdi.h"
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
             vector<int> minimo;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) minimo.push_back((int)f.at<uchar>(x-i,y-j));
                     else if(((x+i)<0)||((x+i)>=f.rows)) minimo.push_back((int)f.at<uchar>(x-i,y+j));
                     else if(((y+i)<0)||((y+i)>=f.cols)) minimo.push_back((int)f.at<uchar>(x+i,y-j));
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else minimo.push_back((int)f.at<uchar>(x+i,y+j));
                 }
             }
             sort(minimo.begin(), minimo.end());
             g.at<uchar>(x,y)=minimo.at(0);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Mínimo", g);
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
             vector<int> maximo;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) maximo.push_back((int)f.at<uchar>(x-i,y-j));
                     else if(((x+i)<0)||((x+i)>=f.rows)) maximo.push_back((int)f.at<uchar>(x-i,y+j));
                     else if(((y+i)<0)||((y+i)>=f.cols)) maximo.push_back((int)f.at<uchar>(x+i,y-j));
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else maximo.push_back((int)f.at<uchar>(x+i,y+j));
                 }
             }
             sort(maximo.begin(), maximo.end());
             g.at<uchar>(x,y)=maximo.at(8);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Máximo", g);
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
    for(int v=0; v<planosHSV.size(); v++) {
        planosHSVCopy.push_back(planosHSV[v]);
    }
    for(int z=0; z<planosHSV.size(); z++)
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
