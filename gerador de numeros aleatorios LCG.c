//livro: Numerical Recipes, 3rd ed. (Press et. al., 2007)
//Capitulo: 7.1.2 Recommended methods for use in combined generators
//Algoritmo: C - LCG modulo 2^64


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>  // Para PRIu64
#include <time.h>

//parâmetros C1 do LCG
#define A 3935559000370003845ULL
#define C 2691343689449507681ULL

#define NUM_GERACOES 4
#define NUM_CATEGORIAS 10

//função inline para utilizar registradores
static inline uint64_t lcg(uint64_t *x) {
    *x = A * (*x) + C;
    return *x;
}

//função que compara a velocidade do lcg com o rand()
static inline void teste_de_velocidade(int *geracoes){
    srand(time(NULL)); //iniciar o rand com a semente baseada no tempo atual
    uint64_t x = time(NULL); //iniciar o lcg baseado no tempo atual

    int i, j;
    printf("\n==========Comparacao de velocidades==========\n");
    for (i = 0; i < NUM_GERACOES; i++){
        printf("\n=== Geracao %d ===\n", geracoes[i]);
        
        clock_t inicio = clock();
        for (j = 0; j < geracoes[i]; j++){
            int rand_num = rand() % 10;
        }
        clock_t fim = clock();
        printf("- Tempo do rand(): %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

        inicio = clock();
        for (j = 0; j < geracoes[i]; j++){
            uint64_t rand_num = lcg(&x);
        }
        fim = clock();
        printf("- Tempo do LCG: %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

    }
}

void inicializar_listas(int frequencias_rand[NUM_GERACOES][NUM_CATEGORIAS], int frequencias_lcg[NUM_GERACOES][NUM_CATEGORIAS]){
    int i,j;
    for (i = 0; i < NUM_GERACOES; i++) {
        for (j = 0; j < NUM_CATEGORIAS; j++) {
            frequencias_rand[i][j] = 0;
            frequencias_lcg[i][j] = 0;
        }
    }
}

void gerar_frequencias(int *geracoes, int frequencias_rand[NUM_GERACOES][NUM_CATEGORIAS], int frequencias_lcg[NUM_GERACOES][NUM_CATEGORIAS]){
    srand(time(NULL)); 
    uint64_t x = time(NULL); 
    int i, j;
    for(i = 0; i < NUM_GERACOES; i++){
        for(j = 0; j < geracoes[i]; j++){
            int rand_num = rand() % 10;
            frequencias_rand[i][rand_num] += 1;
        }
        for(j = 0; j < geracoes[i]; j++){
            uint64_t rand_num = lcg(&x);
            frequencias_lcg[i][rand_num % 10] += 1;
        }
    }
}

void imprimir_frequencias(int *geracoes, int frequencias_rand[NUM_GERACOES][NUM_CATEGORIAS], int frequencias_lcg[NUM_GERACOES][NUM_CATEGORIAS]){
    int i,j;
    printf("\n==========Comparacao de Frequencias==========");
    for (i = 0; i < NUM_GERACOES; i++) {
        printf("\n\n=== Geracao %d ===\n", geracoes[i]);
        printf("==RAND==\n");
        for (j = 0; j < NUM_CATEGORIAS; j++) {
            printf(" %d - ", frequencias_rand[i][j]);
        }
        printf("\n==LCG==\n");
        for (j = 0; j < NUM_CATEGORIAS; j++) {
            printf(" %d - ", frequencias_lcg[i][j]);
        }
    }
}

void teste_chi_quadrado(float *chi_rand, float *chi_lcg, int *geracoes, int frequencias_rand[NUM_GERACOES][NUM_CATEGORIAS], int frequencias_lcg[NUM_GERACOES][NUM_CATEGORIAS]){
    int i, j;
    for(int i = 0; i < NUM_GERACOES; i++){
        float esperado = geracoes[i]/NUM_CATEGORIAS;
        for(int j = 0; j < NUM_CATEGORIAS; j++){
            chi_rand[i] += ((frequencias_rand[i][j] - esperado)*(frequencias_rand[i][j] - esperado))/esperado;
            chi_lcg[i] += ((frequencias_lcg[i][j] - esperado)*(frequencias_lcg[i][j] - esperado))/esperado;
        }
    }
}

float valor_critico(int grau_de_liberdade, float nivel_significancia){
    return 16.92;
}

void imprimir_chi_quadrado(float *chi_rand, float *chi_lcg, int *geracoes){
    int i;
    int grau_de_liberdade = NUM_CATEGORIAS - 1;
    float nivel_significancia = 5/100;
    float valorcritico = valor_critico(grau_de_liberdade, nivel_significancia);
    for(int i = 0; i < NUM_GERACOES; i++){
        printf("\n==========CHI QUADRADO==========\n");
        printf("\n=== Geracao %d ===\n", geracoes[i]);
        printf("* Chi-quadrado RAND = %f - ", chi_rand[i]);
        if(chi_rand[i] > valorcritico){
            printf("REJEITA HIPOTESE NULA \n");
        }else{
            printf("ACEITA HIPOTESE NULA \n");
        }
        printf("\n* Chi-quadrado LCG = %f - ", chi_lcg[i]);
        if(chi_rand[i] > valorcritico){
            printf("REJEITA HIPOTESE NULA \n");
        }else{
            printf("ACEITA HIPOTESE NULA \n");
        }
    }
}

int main() {
    int geracoes[NUM_GERACOES] = {100000, 1000000, 10000000, 100000000};
    int frequencias_rand[NUM_GERACOES][NUM_CATEGORIAS];
    int frequencias_lcg[NUM_GERACOES][NUM_CATEGORIAS];
    float chi_rand[NUM_GERACOES] = {0,0,0,0};
    float chi_lcg[NUM_GERACOES] = {0,0,0,0};
    //comparação de velocidades
    //teste_de_velocidade(geracoes);
    inicializar_listas(frequencias_rand, frequencias_lcg);
    gerar_frequencias(geracoes, frequencias_rand, frequencias_lcg);
    imprimir_frequencias(geracoes,frequencias_rand,frequencias_lcg);  
    teste_chi_quadrado(chi_rand, chi_lcg, geracoes, frequencias_rand, frequencias_lcg);
    imprimir_chi_quadrado(chi_rand, chi_lcg, geracoes);
    return 0;
}
