#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PESO 1000

int main() {
    Solucao sol, sol2;

    ler_arquivo("../instancia-toy.txt");
    //imprimir_dados_arquivo();

    for (int i=1; i<=num_cli; i++) {
        printf(" %d ", vet_dem_cli[i]);
    }

    //gerar_solucao_gulosa(sol);
    //calcular_fo(sol);
    //imprimir_solucao(sol);
/*
    memcpy(&sol2, &sol, sizeof(sol2));
    for (int i=0; i<10; i++) {
        gerar_vizinho(sol2);
        imprimir_solucao(sol2);
    }
*/
    return 0;
}

void gerar_solucao(Solucao& sol) {
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));

    int vei;
    for (int i=1; i<=num_cli; i++) {
        vei = rand() % num_vei;
        sol.mat_sol[vei][sol.vet_aux[vei]] = i;

        sol.vet_aux[vei]++;
    }
}

void gerar_solucao_aleatoria(Solucao& sol) {
    //ALTERAÇÃO!!!!!!!! fazer aleatório a posição para inserir o cliente
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));

    int vei;
    for (int i=1; i<=num_cli; i++) {
        vei = rand() % num_vei;
        sol.mat_sol[vei][sol.vet_aux[vei]] = i;

        sol.vet_aux[vei]++;
    }
}
/*
void gerar_matriz_ordenada(int&& mat_ord_dis) {
    for (int i=0; i<num_vei; i++) {
        for (int j=0; j<num_cli; j++) {
            for (int k=j; k<num_cli; k++) {
                if(mat_dis_cli[i][])
            }
        }
    }
}
*/
void gerar_solucao_gulosa(Solucao& sol) {
    printf("\n\nVetor de demanda: \n");
    for (int i=1; i<=num_cli; i++) {
        printf(" %d ", vet_dem_cli[i]);
    }

    //gerar solução com base na capacidade dos veículos
    int vet_dem_vei[MAX_VEI], vet_ord_cli[MAX_CLI], vet_cop_dem_cli[MAX_CLI];
    
    memset(&vet_dem_vei, 0, sizeof(vet_dem_vei));
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&vet_ord_cli, 0, sizeof(vet_ord_cli));
    memcpy(&vet_cop_dem_cli, &vet_dem_cli, sizeof(vet_dem_cli));

    printf("\n\n");

    //ordenar clientes
    for (int i=1; i<=num_cli; i++) {
        for (int j=1; j<=num_cli; j++) {
            if (vet_cop_dem_cli[ vet_ord_cli[i-1] ] < vet_cop_dem_cli[ j ] && vet_cop_dem_cli[ vet_ord_cli[i] ] != -1) {
                printf("[%d][%d]: %d\n", i, j, vet_cop_dem_cli[ j ]);
                vet_ord_cli[i-1] = j;
            }
        }
        vet_cop_dem_cli[ vet_ord_cli[i-1] ] = -1;
    }

    printf("\nVetor aux das pos dos clientes: \n");
    for (int i=1; i<=num_cli; i++) {
        printf("%d ", vet_ord_cli[i]);
    }

    for (int i=1; i<=num_cli; i++) {
        for (int j=0; j<num_vei; j++) {
            printf("demanda veiculo: %d   capacidade veiculo: %d\n", vet_dem_vei[j], vet_cap_vei[j]);
            if (vet_dem_vei[j] <= vet_cap_vei[j]) {
                sol.mat_sol[j][sol.vet_aux[j]] = i;
                sol.vet_aux[j]++;
                vet_dem_vei[j] += vet_dem_cli[i];
                //printf("%d: %d \n", j, vet_dem_cli[i]);
                break;
            }
        }
    }
}

void gerar_solucao_aleatoria_gulosa(Solucao& sol) {

}

void calcular_fo(Solucao& sol) {
    sol.fo = 0;

    //custo do depósito até o primeiro cliente
    for (int i=0; i<num_vei; i++) {
        sol.fo += mat_dis_cli[0][sol.mat_sol[i][0]];
    }

    //custo entre os clientes e o trajeto final até o depósito
    int capacidade;
    for (int i=0; i<num_vei; i++) {
        capacidade = 0;
        for (int j=0; j<sol.vet_aux[i]; j++) {

            capacidade += vet_dem_cli[ sol.mat_sol[i][j] ];
            
            if (capacidade > vet_cap_vei[i]) {
                sol.fo += PESO * mat_dis_cli[sol.mat_sol[i][j]][sol.mat_sol[i][j+1]];
            } else {
                sol.fo += mat_dis_cli[sol.mat_sol[i][j]][sol.mat_sol[i][j+1]];
            }
            
        }
    }
}

void gerar_vizinho(Solucao& sol) {
    int vei_pre, vei_pos, pos_cliente, cliente;
    vei_pre = rand() % num_vei;

    pos_cliente = rand() % sol.vet_aux[vei_pre];
    cliente = sol.mat_sol[vei_pre][pos_cliente];

    //remover cliente do vei_pre
    sol.mat_sol[vei_pre][pos_cliente] = 0;
    while (sol.mat_sol[vei_pre][pos_cliente+1] != 0) {
        sol.mat_sol[vei_pre][pos_cliente] = sol.mat_sol[vei_pre][pos_cliente+1];
        sol.mat_sol[vei_pre][pos_cliente+1] = 0;
        pos_cliente++;
    }
    sol.vet_aux[vei_pre]--;

    //inserir cliente no veiculo
    do {
        vei_pos = rand() % num_vei;
    } while (vei_pre == vei_pos);

    sol.mat_sol[vei_pos][sol.vet_aux[vei_pos]] = cliente;
    sol.vet_aux[vei_pos]++;
}

void ler_arquivo(char* path) {
    FILE* f = fopen(path, "r");
    int x[MAX_CLI], y[MAX_CLI];

    fscanf(f, "%d %d", &num_cli, &num_vei);

    for (int i=0; i<=num_cli; i++) {
        fscanf(f, "%d %d %d %d", &x[i], &x[i], &y[i], &vet_dem_cli[i]);
    }

    for (int i=0; i<num_vei; i++) {
        fscanf(f, "%d", &vet_cap_vei[i]);
    }

    for (int i=0; i<=num_cli; i++) {
        for (int j=0; j<=num_cli; j++) {
            mat_dis_cli[i][j] = sqrt(pow(( x[i] - x[j] ), 2) + pow(( y[i] - y[j] ), 2));
        }
    }

    fclose(f);
}

/*
void inserir_cliente(Solucao& sol, int& pessoa, int& veiculo) {
    int i;
    for (i = sol.vet_aux[veiculo] - 1; i>=0; i--) {
        if (vet_hora_ini[sol.mat_sol[mot][i]] > vet_hora_ini[tarefa]) {
            sol.mat_sol[mot][i + 1] = sol.mat_sol[mot][i];
        } else {
            break;
        }
    }

    sol.mat_sol[mot][i + 1] = tarefa;
    sol.aux[mot]++;
}

void remover_cliente(Solucao& sol, int& pos, int& mot) {
    for (int i=pos; i<sol.aux[mot] - 1; i++) {
        sol.mat_sol[mot][i] = sol.mat_sol[mot][i+1];
    }

    sol.aux[mot]--;
}
*/

void imprimir_solucao(Solucao& sol) {
    printf("\n-----------------------------------------\n");
    printf("Dados da solucao: \n");
    printf("\nValor da fo: %f\n", sol.fo);

    printf("\nMatriz solucao: \n");
    for (int i=0; i<num_vei; i++) {
        for (int j=0; j<sol.vet_aux[i]; j++) {
            printf("%d ", sol.mat_sol[i][j]);
        }
        printf("\n");
    }

    printf("\nVetor auxiliar: \n");
    for (int i=0; i<num_vei; i++) {
        printf("%d ", sol.vet_aux[i]);
    }
}

void imprimir_dados_arquivo() {
    printf("\n-------------------------------------------------------------\n");
    printf("\nDados do arquivo: \n");
    printf("Clientes: %d      Veiculos: %d\n", num_cli, num_vei);

    printf("\nMatriz das distancias: \n");
    for (int i=0; i<=num_cli; i++) {
        for (int j=0; j<=num_cli; j++) {
            printf("%8.2f ", mat_dis_cli[i][j]);
        }
        printf("\n");
    }

    printf("\nVetor de capacidade dos veiculos:\n");
    for (int i=0; i<num_vei; i++) {
        printf("%d ", vet_cap_vei[i]);
    }

    printf("\nVetor de demanda dos clientes: \n");
    for (int i=0; i<=num_cli; i++) {
        printf("%d ", vet_dem_cli[i]);
    }
    printf("\n-------------------------------------------------------------\n");
}