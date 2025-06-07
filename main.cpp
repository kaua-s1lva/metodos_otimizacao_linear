#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PES_ORI_HUB 1
#define PES_HUB_HUB 0.75
#define PES_HUB_DES 1

int main() {
    //srand(time(NULL));

    Solucao sol;

    ler_arquivo("../inst20.txt");
    //imprimir_dados_arquivo();

    num_hub = MAX(0.2 * num_nos, 2);

    gerar_solucao(sol);
    calcular_solucao(sol);
    imprimir_solucao(sol);

    return 0;
}

void gerar_solucao(Solucao& sol) {
    //determinar quais serão hubs
    for (int i = 0; i<num_hub; i++) {
        sol.vet_hub[i] = rand() % num_nos;
    }
}

void calcular_solucao(Solucao& sol) {
    memset(&sol.mat_sol, -1, sizeof(sol.mat_sol));
    sol.fo = 0;

    int aux = 0;
    float mat_aux[MAX_NOS * MAX_NOS][5];

    //gerando a tabela de possibilidades
    for (int i=0; i<pow(num_nos, 2); i++) {
        for (int j=0; j<pow(num_hub, 2); j++) {
            //origem
            mat_aux[aux][0] = (int) ((float)aux / (num_nos * pow(num_hub, 2)));

            //hub1
            mat_aux[aux][1] = sol.vet_hub[j / num_hub];

            //hub2
            mat_aux[aux][2] = sol.vet_hub[j % num_hub];

            //destino
            mat_aux[aux][3] = i % num_nos;

            //custo
            mat_aux[aux][4] = 
                (mat_dis[(int) mat_aux[aux][0]][(int) mat_aux[aux][1]] * PES_ORI_HUB) + 
                (mat_dis[(int) mat_aux[aux][1]][(int) mat_aux[aux][2]] * PES_HUB_HUB) + 
                (mat_dis[(int) mat_aux[aux][2]][(int) mat_aux[aux][3]] * PES_HUB_DES)
            ;

            aux++;
        }
    }

    aux = 0;
    //calculando matriz solução
    for (int i=0; i<pow(num_nos, 2); i++) {
        for (int j=0; j<pow(num_hub, 2); j++) {
            if (sol.mat_sol[i][4] < mat_aux[i+1][4]) {
                sol.mat_sol[aux][0] = mat_aux[aux][0];
                sol.mat_sol[aux][1] = mat_aux[aux][1];
                sol.mat_sol[aux][2] = mat_aux[aux][2];
                sol.mat_sol[aux][3] = mat_aux[aux][3];
                sol.mat_sol[aux][4] = mat_aux[aux][4];
            }
            aux++;
        }
    }

    //setando valores iniciais
    for (int i=0; i < (pow(num_nos, 2)); i++) {
        for (int j=0; j < 5; j++) {
            sol.mat_sol[i][j] = mat_aux[i * (int) pow(num_hub, 2)][j];
        }
    }

    //setando valores corretos para a matriz solução
    aux = 0;
    for (int i=0; i<pow(num_nos, 2); i++) {
        for (int j=0; j<pow(num_hub, 2); j++) {
            if (sol.mat_sol[i][4] > mat_aux[aux][4]) {
                sol.mat_sol[i][0] = mat_aux[aux][0];
                sol.mat_sol[i][1] = mat_aux[aux][1];
                sol.mat_sol[i][2] = mat_aux[aux][2];
                sol.mat_sol[i][3] = mat_aux[aux][3];
                sol.mat_sol[i][4] = mat_aux[aux][4];
            }
            aux++;
        }
    }

    //calculando a fo
    for (int i=0; i < (pow(num_nos, 2)); i++) {
        if (sol.mat_sol[i][4] > sol.fo) {
            sol.fo = sol.mat_sol[i][4];
        }
    }
/*
    printf("\nMatriz auxiliar: \n");
    for (int i=0; i < (pow(num_hub, 2) * pow(num_nos, 2)); i++) {

        for (int j=0; j<5; j++) {
            printf("%5.2f ", mat_aux[i][j]);
        }
        printf("\n");
    }
*/

}

void ler_arquivo(char* path) {
    FILE* f = fopen(path, "r");
    float x[MAX_NOS];
    float y[MAX_NOS];

    fscanf(f, "%d", &num_nos);
    for (int i=0; i<num_nos; i++) {
        fscanf(f, "%f %f", &x[i], &y[i]);
    }

    for (int i=0; i<num_nos; i++) {
        for (int j=0; j<num_nos; j++) {
            mat_dis[i][j] = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
        }
    }

    fclose(f);
}

void imprimir_solucao(Solucao& sol) {
    printf("\nMatriz solucao: \n");
    for (int i=0; i < (pow(num_nos, 2)); i++) {
        for (int j=0; j<5; j++) {
            printf("%10.2f ", sol.mat_sol[i][j]);
        }
        printf("\n");
    }

    printf("\nn: %d      p: %d", num_nos, num_hub);
    printf("\nFO: %.2f", sol.fo);
    printf("\nHubs: [ ");
    for (int i=0; i<num_hub; i++) {
        printf("%d ", sol.vet_hub[i]);
    }
    printf("]");
}

void imprimir_dados_arquivo() {
    printf("Numero de Nos: %d\n", num_nos);
/*
    for (int i=0; i<num_nos; i++) {
        printf("%5.2f %5.2f\n", x[i], y[i]);
    }
*/
    for (int i=0; i<num_nos; i++) {
        for (int j=0; j<num_nos; j++) {
            printf("%10.2f ", mat_dis[i][j]);
        }
        printf("\n");
    }
}