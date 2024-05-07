#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_FILENAME 256

typedef struct {
    int maximalBranco;
    int maximalPreto;
} maximais;

int quantidadeMaximais(char** matriz, int inicioLinha, int fimLinha, int inicioColuna, int fimColuna, maximais* quadrante)
{
    int pixels = 0;
    if (inicioLinha < fimLinha && inicioColuna < fimColuna) {
        int meioLinha = (inicioLinha + fimLinha) / 2;
        int meioColuna = (inicioColuna + fimColuna) / 2;

        pixels += quantidadeMaximais(matriz, meioLinha + 1, fimLinha, meioColuna + 1, fimColuna, quadrante);
        pixels += quantidadeMaximais(matriz, meioLinha + 1, fimLinha, inicioColuna, meioColuna, quadrante);
        pixels += quantidadeMaximais(matriz, inicioLinha, meioLinha, meioColuna + 1, fimColuna, quadrante);
        pixels += quantidadeMaximais(matriz, inicioLinha, meioLinha, inicioColuna, meioColuna, quadrante);

    } else {
        if (matriz[inicioLinha][inicioColuna] == '0') {
            quadrante[0].maximalPreto++;
            return 0;
        }
        else {
            quadrante[0].maximalBranco++;
            return 1;
        }
    }

    int tamanhoQuadrante = fimLinha - inicioLinha + 1;
    int elementosQuadrante = pow(tamanhoQuadrante, 2);
    int potencia = log(tamanhoQuadrante) / log(2);

    if (pixels == elementosQuadrante) {
        quadrante[potencia].maximalBranco++;
        quadrante[potencia - 1].maximalBranco -= 4;
    } else if (pixels == 0) {
        quadrante[potencia].maximalPreto++;
        quadrante[potencia - 1].maximalPreto -= 4;
    }

    return pixels;
}

int main(int argc, char *argv[])
{
    int tamanho;
    sscanf(argv[1], "%i", &tamanho);

    int numeroQuadrantes = log(tamanho) / log(2) + 1;
    maximais* quadrante = (maximais*) calloc(sizeof(maximais), numeroQuadrantes);

    char file[MAX_FILENAME] = "";
    strcpy(file, argv[2]);
    FILE* fp = fopen(file, "r");

    char** imagem = (char**) calloc(sizeof(char*), tamanho + 1);
    for (int i = 0; i < tamanho; i++) {
        imagem[i] = (char*) calloc(sizeof(char), tamanho + 1);
        fscanf(fp, "%s", imagem[i]);
    }
    fclose(fp);

    int fim = tamanho - 1;
    int inicio = 0;
    quantidadeMaximais(imagem, inicio, fim, inicio, fim, quadrante);

    int dimensaoQuadrante = tamanho;
    FILE *saida = fopen("saida.txt", "w");
    for (int i = numeroQuadrantes - 1; i >= 0; i--) {
        fprintf(saida, "%ix%i %i %i\n", dimensaoQuadrante, dimensaoQuadrante, quadrante[i].maximalBranco, quadrante[i].maximalPreto);
        dimensaoQuadrante /= 2;
    }
    fclose(saida);

    for (int i = 0; i < tamanho; i++) {
        if (imagem[i] != NULL) free(imagem[i]);
        imagem[i] = NULL;
    }
    if (imagem != NULL) free(imagem);
    imagem = NULL;

    if (quadrante != NULL) free(quadrante);
    quadrante = NULL;

    return 0;
}
