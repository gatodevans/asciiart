#include <iostream>
#include <string.h>
#include <cmath>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* reduzirImagem(unsigned char* img, int larg, int alt, int canais,
                             int novaLarg, int novaAlt)
{
    unsigned char* novaImg = (unsigned char*)malloc(novaLarg * novaAlt * canais);
    float scaleX = (float)larg / novaLarg;
    float scaleY = (float)alt / novaAlt;
    for(int y = 0; y < novaAlt; y++){
        for(int x = 0; x < novaLarg; x++){
            int origX = (int)(x * scaleX);
            int origY = (int)(y * scaleY);

            int idxNova = (y * novaLarg + x) * canais;
            int idxOrig = (origY * larg + origX) * canais;

            novaImg[idxNova + 0] = img[idxOrig + 0];
            novaImg[idxNova + 1] = img[idxOrig + 1];
            novaImg[idxNova + 2] = img[idxOrig + 2];
        }
    }
    return novaImg;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cout << "Uso: ./ascii <imagem>\n";
        return 1;
    }

    char Letras[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i! lI;:,";
    int qntdletras = strlen(Letras);
    int largura, altura, canais;
    unsigned char *Imagem = stbi_load(argv[1], &largura, &altura, &canais, 3);
    if(!Imagem){
        std::cerr << "Erro ao carregar imagem.\n";
        return 1;
    }
    int novaLarg = 500;                 
    float proporcao = (float)novaLarg / largura;
    int novaAlt = (int)(altura * proporcao * 0.5f);


    unsigned char* reduzida = reduzirImagem(Imagem, largura, altura, 3, novaLarg, novaAlt);
    unsigned char* pixels = reduzida;
    for(int y = 0; y < novaAlt; y++){
        for(int x = 0; x < novaLarg; x++){
            unsigned char R = *pixels++;
            unsigned char G = *pixels++;
            unsigned char B = *pixels++;
            float media = (R + G + B) / 3.0f;
            int idx = (int)(qntdletras * (media / 255.0f));
            if(idx >= qntdletras) idx = qntdletras - 1;
            std::cout << "\x1b[38;2;" << (int)R << ";" << (int)G << ";" << (int)B << "m"
                      << Letras[idx]
                      << "\x1b[0m";
        }
        std::cout << "\n";
    }

    stbi_image_free(Imagem);
    free(reduzida);
    return 0;
}
