#include <stdio.h>
#include <memory.h>
#include "header.h"

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PEN 100

void calcular_horas_extras(Solucao& sol) {
    int total = 0;
    for (int i=0; i<MAX_MOT; i++) {
        total += MAX(sol.vet_hora_trab[i] - temp_norm_trab, 0);
        sol.fo += MAX(sol.vet_hora_trab[i] - temp_norm_trab, 0);
    }
    printf("\nCalculo de horas extras: %d\n", total);
}

void calcular_tempo_ocioso(Solucao& sol) {
    int total = 0;
    for (int i=0; i<MAX_MOT; i++) {
        for (int j=0; j<sol.aux[i]-1; j++) {
            total += vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]];
            sol.fo += vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]];
        }
    }
    printf("\nCalculo de tempo ocioso: %d\n", total);
}

void calcular_tempo_sobreposicao(Solucao& sol) {
    int total = 0;
    for (int i=0; i<MAX_MOT; i++) {
        for (int j=0; j<sol.aux[i]-1; j++) {
            total += MIN(vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]], 0) * (-PEN);
            sol.fo += MIN(vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]], 0) * (-PEN);
        }
    }
    printf("\nCalculo de tempo de sobreposicao: %d\n", total);
}

void calcular_tempo_acima_max(Solucao& sol) {
    int total = 0;
    for (int i=0; i<MAX_MOT; i++) {
        total += MAX(sol.vet_hora_trab[i] - temp_max_trab, 0) * PEN;
        sol.fo += MAX(sol.vet_hora_trab[i] - temp_max_trab, 0) * PEN;
    }
    printf("\nCalculo de tempo acima do limite: %d\n", total);
}

int main() {
    Solucao sol;

    ler_arquivo("../csp25.txt");
    //imprimir_dados_arquivo();
    criar_solucao(sol);
    calcular_fo_solucao(sol);
    imprimir_solucao(sol);
    printf("\nValor da fo: %d", sol.fo);

    return 0;
}

void ler_arquivo(char* path) {
    FILE* f = fopen(path, "r");

    fscanf(f, "%d %d %d", &num_tarefas, &temp_norm_trab, &temp_max_trab);
    for (int i=0; i<MAX_TAR; i++) {
        fscanf(f, "%d %d", &vet_hora_ini[i], &vet_hora_fim[i]);
    }

    fclose(f);
}

void imprimir_dados_arquivo() {
    printf("%d %d %d\n", num_tarefas, temp_norm_trab, temp_max_trab);
    for (int i=0; i<MAX_TAR; i++) {
        printf("%d %d\n", vet_hora_ini[i], vet_hora_fim[i]);
    }
}

void criar_solucao(Solucao& sol) {
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));
    memset(&sol.aux, 0, sizeof(sol.aux));

    int aux = 0;
    for (int i=0; i<num_tarefas; i++) {
        aux = i % MAX_MOT;
        sol.mat_sol[aux][sol.aux[aux]] = i;
        sol.aux[aux]++;
    }

}

void calcular_fo_solucao(Solucao& sol) {
    sol.fo = 0;

    memset(&sol.vet_hora_trab, 0, sizeof(sol.vet_hora_trab));

    for (int i=0; i<MAX_MOT; i++) {
        for (int j=0; j<sol.aux[i]; j++) {
            sol.vet_hora_trab[i] += vet_hora_fim[sol.mat_sol[i][j]] - vet_hora_ini[sol.mat_sol[i][j]];
        }
    }

    calcular_horas_extras(sol);

    calcular_tempo_ocioso(sol);

    calcular_tempo_sobreposicao(sol);

    calcular_tempo_acima_max(sol);
}

void imprimir_solucao(Solucao& sol) {
    printf("Matriz solucao: \n");
    for (int i=0; i<MAX_MOT; i++) {
        printf("%4d |", i);
        for (int j=0; j<MAX_TAR; j++) {
            printf("%4d ", sol.mat_sol[i][j]);
        }
        printf("\n");
    }

    printf("\nVetor auxiliar: \n");
    for (int i=0; i<MAX_MOT; i++) {
        printf("%3d ", i);
    }
    printf("\n");
    for (int i=0; i<MAX_MOT; i++) {
        printf("%3d ", sol.aux[i]);
    }

    printf("\nValor da fo: %d", sol.fo);
}