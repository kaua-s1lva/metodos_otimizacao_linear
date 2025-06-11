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
    imprimir_dados_arquivo();

    gerar_solucao(sol);
    calcular_fo(sol);
    imprimir_solucao(sol);

    memcpy(&sol2, &sol, sizeof(sol2));
    for (int i=0; i<10; i++) {
        gerar_vizinho(sol2);
        imprimir_solucao(sol2);
    }

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
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));

    int vei;
    for (int i=1; i<=num_cli; i++) {
        vei = rand() % num_vei;
        sol.mat_sol[vei][sol.vet_aux[vei]] = i;

        sol.vet_aux[vei]++;
    }
}

void gerar_solucao_gulosa(Solucao& sol) {
    /*
        IDEIA!!!!!!!!!!
        Criar uma matriz ordenada para armazenar as posições dos clientes que estão menos longe
        [
            [0] => [0, 2, 1, 3]
            [1] => [0, 1, 3, 2]
            ...
        ]
    */


    int j, mat_dis_cpy[MAX_VEI][MAX_CLI];
    float distancia = mat_dis_cli[0][1];

    memcpy(&mat_dis_cpy, &mat_dis_cli, sizeof(mat_dis_cpy));

    //depósito -> primeiro cliente
    for (int i=0; i<num_vei; i++) {
        for (j=1; j<=num_cli; j++) {
            if (distancia > mat_dis_cpy[0][j] && mat_dis_cpy[0][j] != -1) {
                distancia = mat_dis_cpy[0][j];
            }
        }
        mat_dis_cpy[0][j] = -1;
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