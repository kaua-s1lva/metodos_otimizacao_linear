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

    ler_arquivo("../inst10.txt");
    imprimir_dados_arquivo();

    num_hub = MAX(0.2 * num_nos, 3);

    criar_solucao_gulosa(sol);
    calcular_solucao(sol);
    imprimir_solucao(sol);
/*
    Solucao sol2;

    memcpy(&sol2, &sol, sizeof(sol2));

    for (int i=0; i<10; i++) {
        gerar_vizinho(sol2);
        calcular_solucao(sol2);
        imprimir_solucao(sol2);
        if (sol2.fo < sol.fo) {
            memcpy(&sol, &sol2, sizeof(sol));
        }
    }
    
    imprimir_solucao(sol);
    */
    return 0;
}

void criar_solucao(Solucao& sol) {
    //determinar quais serão hubs
    for (int i = 0; i<num_hub; i++) {
        sol.vet_hub[i] = rand() % num_nos;
    }
}

void criar_solucao_aleatoria(Solucao& sol) {
    for (int i = 0; i<num_hub; i++) {
        sol.vet_hub[i] = rand() % num_nos;
    }
}

void criar_solucao_gulosa(Solucao& sol) {
    //Posso adotar que, quanto mais separado os hubs, melhor a distribuição
    memset(&sol.vet_hub, 0, sizeof(sol.vet_hub));
    int x=0, y=0, aux, vet_nos[MAX_NOS], i=2, hubs=2;

    for (int i=0; i<num_nos; i++) {
        vet_nos[i] = i;
    }

    //achando a posição do primeiro e segundo hub
    for (int i=0; i<num_nos; i++) {
        for (int j=0; j<i; j++) {
            if (mat_dis[sol.vet_hub[0]][sol.vet_hub[1]] < mat_dis[i][j]) {
                sol.vet_hub[0] = i;
                sol.vet_hub[1] = j;
            }
        }
    }

    aux = 0;

    //achando a posição dos demais hubs
    bool jaEscolhido = false;
    while (hubs < num_hub) {
        aux = 0;
        for (int i=0; i<hubs; i++) {
            for (int j=0; j<num_nos; j++) {
                jaEscolhido = false;
                for (int k=0; k<hubs; k++) {
                    if (j == sol.vet_hub[k]) {
                        jaEscolhido = true;
                        break;
                    }
                }

                if (jaEscolhido) continue;
    
                if (aux < mat_dis[i][j]) {
                    aux = mat_dis[i][j];
                    x = i;
                    y = j;
                }
            }
        }
        sol.vet_hub[hubs] = y;
        hubs++;
    }


}

void criar_solucao_aleatoria_gulosa(Solucao& sol) {
    
}

void calcular_solucao(Solucao& sol) {
    memset(&sol.mat_sol, -1, sizeof(sol.mat_sol));
    sol.fo = 0;

    int aux = 0;
    float mat_aux[MAX_NOS * MAX_NOS][5];

    //gerando a tabela de todas as possibilidades (matriz auxiliar)
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

    //setando valores iniciais
    aux = 0;
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
    
}

void gerar_vizinho(Solucao& sol) {
    int pos, hub;
    pos = rand() % num_hub;
    hub = sol.vet_hub[pos];

    while (hub == sol.vet_hub[pos]) {
        sol.vet_hub[pos] = rand() % num_nos;
    }
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
            printf("%8.2f ", sol.mat_sol[i][j]);
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
    /*
    printf("Numero de Nos: %d\n", num_nos);

    for (int i=0; i<num_nos; i++) {
        for (int j=0; j<num_nos; j++) {
            printf("%10.2f ", mat_dis[i][j]);
        }
        printf("\n");
    }
    */

    FILE* f = fopen("../mat_dis.csv", "w");
    //fprintf(f, "Numero de Nos: %d\n", num_nos);

    for (int i=0; i<num_nos; i++) {
        for (int j=0; j<num_nos; j++) {
            fprintf(f, "%.0f/", mat_dis[i][j]);
        }
        fprintf(f, "\n");
    }
}