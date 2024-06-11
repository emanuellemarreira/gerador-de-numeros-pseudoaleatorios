//livro: Numerical Recipes, 3rd ed. (Press et. al., 2007)
//Capitulo: 7.1.2 Recommended methods for use in combined generators
//Algoritmo: C - LCG modulo 2^64

// -*- coding: utf-8 -*-
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>  // Para PRIu64
#include <time.h>

//parâmetros C1 do LCG
#define A 3935559000370003845ULL
#define C 2691343689449507681ULL

#define NUM_GERACOES 4

//função inline para utilizar registradores
static inline uint64_t lcg(uint64_t *x) {
    *x = A * (*x) + C;
    return *x%10; //utiliza mod 10 para que os resultados se mantenham entre 0 e 9
}

//funcao que compara a velocidade do lcg com o rand()
static inline void teste_de_velocidade(int *geracoes){
    srand(time(NULL)); //iniciar o rand com a semente baseada no tempo atual
    uint64_t x = time(NULL); //iniciar o lcg baseado no tempo atual

    int i, j;
    for (i = 0; i < NUM_GERACOES; i++){
        printf("\n***Comparacao de velocidades - geracao %d ***\n", geracoes[i]);
        
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

int main() {
    //Estado inicial
    uint64_t x = 123456777; 

    int geracoes[NUM_GERACOES] = {100000, 1000000, 10000000, 100000000};

   //geração de 10 numeros pseudo-aleatorios
    printf("geracao de 10 numeros pseudo-aleatorios a partir do state: %" PRIu64 "\n", x);
    for (int i = 0; i < 10; i++) {
        uint64_t rand_num = lcg(&x);
        printf("%" PRIu64 "-", rand_num);
    }

    //comparacao de velocidades
    teste_de_velocidade(geracoes);

    return 0;
}
