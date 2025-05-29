#include <stdio.h>
#include <memory.h>
#include "header.h"

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PEN 100

int main() {
    Solucao sol;

    ler_arquivo("../csp25.txt");
    //imprimir_dados_arquivo();
    criar_solucao(sol);
    calcular_fo_solucao(sol);
    imprimir_solucao(sol);

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
    sol.hora_extra = sol.temp_exces = sol.temp_ocios = sol.temp_sobre = 0;

    for (int i=0; i<MAX_MOT; i++) {
        for (int j=0; j<sol.aux[i]; j++) {
            //--------------------------CALCULAR HORA TRABALHADA--------------------------------------------
            sol.vet_hora_trab[i] += vet_hora_fim[sol.mat_sol[i][j]] - vet_hora_ini[sol.mat_sol[i][j]];

            //--------------------------CALCULAR TEMPO OCIOSO ENTRE TAREFAS---------------------------------
            sol.temp_ocios += vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]];

            //--------------------------CALCULAR TEMPO DE SOBREPOSIÇÃO--------------------------------------
            sol.temp_sobre += MIN(vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]], 0) * (-PEN);
        }
        //------------------------------CALCULAR HORA EXTRA-------------------------------------------------
        sol.hora_extra += MAX(sol.vet_hora_trab[i] - temp_norm_trab, 0);

        //------------------------------CALCULAR TEMPO EXCESSIVO--------------------------------------------
        sol.temp_exces += MAX(sol.vet_hora_trab[i] - temp_max_trab, 0) * PEN;
    }

    sol.fo = sol.temp_ocios + sol.temp_sobre + sol.hora_extra + sol.temp_exces;
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

    printf("\nValor TEMPO OCIOSO ENTRE TAREFAS: %d\n", sol.temp_ocios);
    printf("\nValor TEMPO DE SOBREPOSICAO: %d\n", sol.temp_sobre);
    printf("\nValor HORA EXTRA: %d\n", sol.hora_extra);
    printf("\nValor TEMPO EXCESSIVO: %d\n", sol.temp_exces);

    printf("\nValor da fo: %d", sol.fo);
}