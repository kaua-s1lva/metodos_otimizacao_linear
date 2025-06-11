#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main() {
    Solucao sol;

    ler_arquivo("../instancia-toy.txt");
    imprimir_dados_arquivo();

    gerar_solucao(sol);
    calcular_fo(sol);
    imprimir_solucao(sol);

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

void calcular_fo(Solucao& sol) {
    sol.fo = 0;

    //custo do depósito até o primeiro cliente
    for (int i=0; i<num_vei; i++) {
        sol.fo += mat_dis_cli[0][sol.mat_sol[i][0]];
    }

    //custo entre os clientes e o trajeto final até o depósito
    for (int i=0; i<num_vei; i++) {
        for (int j=0; j<sol.vet_aux[i]; j++) {
            sol.fo += mat_dis_cli[sol.mat_sol[i][j]][sol.mat_sol[i][j+1]];
        }
    }

    printf("\nValor da fo: %f", sol.fo);
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

void imprimir_solucao(Solucao& sol) {
    printf("\n-----------------------------------------\n");
    printf("Dados da solucao: \n");

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