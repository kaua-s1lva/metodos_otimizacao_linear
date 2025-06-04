#include <stdio.h>
#include <memory.h>
#include "header.h"
#include <stdlib.h>
#include <time.h>

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PEN 100

int main() {
    Solucao sol, sol2;

    //srand(time(NULL));

    ler_arquivo("../csp25.txt");
    //imprimir_dados_arquivo();
    criar_solucao(sol);
    calcular_fo_solucao(sol);
    //imprimir_solucao(sol);

    memcpy(&sol2, &sol, sizeof(sol2));
    imprimir_solucao(sol2);

    for (int i=0; i<1000; i++) {
        gerar_vizinho(sol2);
        printf("\n%d: %d", i, sol2.fo);
        if (sol2.fo <= sol.fo) {
            memcpy(&sol, &sol2, sizeof(sol));
            printf("\nVALOR ENCONTRADO: %d: %d", i, sol2.fo);
        }
    }

    printf("\nSolucao 1:\n");
    imprimir_solucao(sol);
    imprimir_solucao(sol2);

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
        for (int j=1; j<sol.aux[i]; j++) {
            //--------------------------CALCULAR HORA TRABALHADA--------------------------------------------
            sol.vet_hora_trab[i] += vet_hora_fim[sol.mat_sol[i][j]] - vet_hora_ini[sol.mat_sol[i][j]];

            //--------------------------CALCULAR TEMPO OCIOSO ENTRE TAREFAS---------------------------------
            sol.temp_ocios += vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]];

            //--------------------------CALCULAR TEMPO DE SOBREPOSIÇÃO--------------------------------------
            sol.temp_sobre += MIN(vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]], 0) * (-PEN);
        }
        //------------------------------CALCULAR HORA EXTRA-------------------------------------------------
        sol.hora_extra += MAX(sol.vet_hora_trab[i] - temp_norm_trab, 0) - MAX(sol.vet_hora_trab[i] - temp_max_trab, 0);

        //------------------------------CALCULAR TEMPO EXCESSIVO--------------------------------------------
        sol.temp_exces += MAX(sol.vet_hora_trab[i] - temp_max_trab, 0) * PEN;

        //--------------------------CALCULAR TEMPO OCIOSO ENTRE A ULTIMA TAREFA E O TEMPO DE TRABALHO-------
        //sol.temp_ocios += vet_hora_ini[sol.mat_sol[i][j+1]] - vet_hora_fim[sol.mat_sol[i][j]];
    }

    sol.fo = sol.temp_ocios + sol.temp_sobre + sol.hora_extra + sol.temp_exces;
}

void gerar_vizinho(Solucao& sol) {
    int mot_pre = rand() % MAX_MOT;
    int mot_pos;
    int pos_tar = rand() % (sol.aux[mot_pre]);
    int tar = sol.mat_sol[mot_pre][pos_tar];

    /*
    for (int i=pos_tar; i<sol.aux[mot_pre]; i++) {
        sol.mat_sol[mot_pre][i] = sol.mat_sol[mot_pre][i+1];
    }
    */
    remover_tarefa(sol, pos_tar, mot_pre);

    do {
        mot_pos = rand() % MAX_MOT;

        //método de inserção
        /*
        int i;
        for (i = sol.aux[mot_pos] - 1; i>=0; i--) {
            if (vet_hora_ini[sol.mat_sol[mot_pos][i]] > vet_hora_ini[tar]) {
                sol.mat_sol[mot_pos][i + 1] = sol.mat_sol[mot_pos][i];
            } else {
                break;
            }
        }

        sol.mat_sol[mot_pos][i + 1] = tar;
        sol.aux[mot_pos]++;
        */
        inserir_tarefa(sol, tar, mot_pos);

        //PROBLEMA: precisa garantir que o mot_pre precisa ser não nulo, ou seja, sol.aux[mot_pre] != 0
    } while (mot_pos == mot_pre || sol.aux[mot_pre] == 0);

    
    calcular_fo_solucao(sol);
    //printf("Valor da fo: %d\n", sol.fo);
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

    printf("\nVetor de horas trabalhadas: \n");
    for (int i=0; i<MAX_MOT; i++) {
        printf("%3d ", sol.vet_hora_trab[i]);
    }

    printf("\nValor TEMPO OCIOSO ENTRE TAREFAS: %d\n", sol.temp_ocios);
    printf("\nValor TEMPO DE SOBREPOSICAO: %d\n", sol.temp_sobre);
    printf("\nValor HORA EXTRA: %d\n", sol.hora_extra);
    printf("\nValor TEMPO EXCESSIVO: %d\n", sol.temp_exces);

    printf("\nValor da fo: %d", sol.fo);
}

void inserir_tarefa(Solucao& sol, int tarefa, int mot) {
    int i;
    for (i = sol.aux[mot] - 1; i>=0; i--) {
        if (vet_hora_ini[sol.mat_sol[mot][i]] > vet_hora_ini[tarefa]) {
            sol.mat_sol[mot][i + 1] = sol.mat_sol[mot][i];
        } else {
            break;
        }
    }

    sol.mat_sol[mot][i + 1] = tarefa;
    sol.aux[mot]++;
}

void remover_tarefa(Solucao& sol, int pos, int mot) {
    for (int i=pos; i<sol.aux[mot]; i++) {
        sol.mat_sol[mot][i] = sol.mat_sol[mot][i+1];
    }

    sol.aux[mot]--;
}