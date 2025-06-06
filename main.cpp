#include <stdio.h>
#include <memory.h>
#include "header.h"
#include <stdlib.h>
#include <time.h>

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PEN_TEM_OCI 1
#define PEN_TEM_SOB 100
#define PEN_HOR_EXT 1
#define PEN_TEM_EXC 100

int main() {
    int duracao = 0;
    Solucao sol, sol2;

    //srand(time(NULL));

    ler_arquivo("../csp25.txt");

    for (int i=0; i<num_tarefas; i++) {
        duracao += vet_hora_fim[i] - vet_hora_ini[i];
    }

    num_motoristas = duracao / temp_norm_trab;

    criar_solucao_gulosa(sol);
    calcular_fo_solucao(sol);
    imprimir_solucao(sol);
/*
    memcpy(&sol2, &sol, sizeof(sol2));

    for (int i=0; i<100000; i++) {
        gerar_vizinho(sol2);
        if (sol2.fo <= sol.fo) {
            memcpy(&sol, &sol2, sizeof(sol));
        }
    }

    imprimir_solucao(sol);
    imprimir_solucao(sol2);
*/
    return 0;
}

void ler_arquivo(char* path) {
    FILE* f = fopen(path, "r");

    fscanf(f, "%d %d %d", &num_tarefas, &temp_norm_trab, &temp_max_trab);
    for (int i=0; i<num_tarefas; i++) {
        fscanf(f, "%d %d", &vet_hora_ini[i], &vet_hora_fim[i]);
    }

    fclose(f);
}

void imprimir_dados_arquivo() {
    printf("%d %d %d\n", num_tarefas, temp_norm_trab, temp_max_trab);
    for (int i=0; i<num_tarefas; i++) {
        printf("%d %d\n", vet_hora_ini[i], vet_hora_fim[i]);
    }
}

void criar_solucao(Solucao& sol) {
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));
    memset(&sol.aux, 0, sizeof(sol.aux));

    int aux = 0;
    for (int i=0; i<num_tarefas; i++) {
        aux = i % num_motoristas;
        sol.mat_sol[aux][sol.aux[aux]] = i;
        sol.aux[aux]++;
    }
}

void criar_solucao_aleatoria(Solucao& sol) {
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));
    memset(&sol.aux, 0, sizeof(sol.aux));

    int mot;
    for (int i=0; i<num_tarefas; i++) {
        mot = rand() % num_motoristas;
        sol.mat_sol[mot][sol.aux[mot]] = i;
        sol.aux[mot]++;
    }
}

void criar_solucao_gulosa(Solucao& sol) {
    memset(&sol.mat_sol, -1, sizeof(sol.mat_sol));
    memset(&sol.aux, 0, sizeof(sol.aux));

    //diminuindo o tempo ocioso
    for (int i=0; i<num_tarefas; i++) {
        for (int j=0; j<num_motoristas; j++) {
            if ( 
                vet_hora_fim[ sol.mat_sol[j][sol.aux[j]] ] <= vet_hora_ini[i] && 
                
            ) {
                sol.mat_sol[j][sol.aux[j]] = i;
                sol.aux[j]++;
                break;
            }
/*
            if (vet_hora_fim[sol.mat_sol[i][j]] < vet_hora_ini[sol.mat_sol[i][j+1]]) {
                sol.mat_sol[i][sol.aux[i]] = j;
            }
*/
        }
    }

    //sem sobreposição


    //sem hora extra
}

void criar_solucao_aleatoria_gulosa(Solucao& sol) {

}

void calcular_fo_solucao(Solucao& sol) {
    sol.fo = 0;

    memset(&sol.vet_hora_trab, 0, sizeof(sol.vet_hora_trab));
    sol.hora_extra = sol.temp_exces = sol.temp_ocios = sol.temp_sobre = 0;

    for (int i=0; i<num_motoristas; i++) {
        if (sol.aux[i] > 0) {
            for (int j=1; j<sol.aux[i]; j++) {
                //--------------------------CALCULAR HORA TRABALHADA--------------------------------------------
                sol.vet_hora_trab[i] += vet_hora_fim[sol.mat_sol[i][j-1]] - vet_hora_ini[sol.mat_sol[i][j-1]];

                //--------------------------CALCULAR TEMPO OCIOSO ENTRE TAREFAS---------------------------------
                sol.temp_ocios += MAX(vet_hora_ini[sol.mat_sol[i][j]] - vet_hora_fim[sol.mat_sol[i][j-1]], 0);

                //--------------------------CALCULAR TEMPO DE SOBREPOSIÇÃO--------------------------------------
                sol.temp_sobre += MIN(vet_hora_ini[sol.mat_sol[i][j]] - vet_hora_fim[sol.mat_sol[i][j-1]], 0) * -1;
            }
            //------------------------------CALCULAR HORA EXTRA-------------------------------------------------
            sol.hora_extra += MAX(sol.vet_hora_trab[i] - temp_norm_trab, 0) - MAX(sol.vet_hora_trab[i] - temp_max_trab, 0);

            //------------------------------CALCULAR TEMPO EXCESSIVO--------------------------------------------
            sol.temp_exces += MAX(sol.vet_hora_trab[i] - temp_max_trab, 0);

            //--------------------------CALCULAR TEMPO OCIOSO ENTRE A ULTIMA TAREFA E O TEMPO DE TRABALHO-------
            sol.temp_ocios += MAX(temp_norm_trab - sol.vet_hora_trab[i], 0);
        }

    }

    sol.fo =    sol.temp_ocios * PEN_TEM_OCI + 
                sol.temp_sobre * PEN_TEM_SOB + 
                sol.hora_extra * PEN_HOR_EXT + 
                sol.temp_exces * PEN_TEM_EXC
    ;
}

void gerar_vizinho(Solucao& sol) {

    int mot_pre, mot_pos, pos_tar, tar;
    do {
        mot_pre = rand() % num_motoristas;
    } while (sol.aux[mot_pre] == 0);

    pos_tar = rand() % (sol.aux[mot_pre]);
    tar = sol.mat_sol[mot_pre][pos_tar];

    do {
        mot_pos = rand() % num_motoristas;
    } while (mot_pos == mot_pre);

    remover_tarefa(sol, pos_tar, mot_pre);
    
    inserir_tarefa(sol, tar, mot_pos);
    
    calcular_fo_solucao(sol);
}

void imprimir_solucao(Solucao& sol) {
    printf("\nMatriz solucao: \n");
    for (int i=0; i<num_motoristas; i++) {
        if (sol.aux[i] > 0) {
            printf("%4d |", i);
            printf(" %4d |", sol.aux[i]);
            for (int j=0; j<sol.aux[i]; j++) {
                printf("%4d ", sol.mat_sol[i][j]);
            }
            printf("\n");
        }
    }

    printf("\nValor TEMPO OCIOSO ENTRE TAREFAS: %d\n", sol.temp_ocios);
    printf("\nValor TEMPO DE SOBREPOSICAO: %d\n", sol.temp_sobre);
    printf("\nValor HORA EXTRA: %d\n", sol.hora_extra);
    printf("\nValor TEMPO EXCESSIVO: %d\n", sol.temp_exces);

    printf("\nValor da fo: %d", sol.fo);
}

void inserir_tarefa(Solucao& sol, int& tarefa, int& mot) {
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

void remover_tarefa(Solucao& sol, int& pos, int& mot) {
    for (int i=pos; i<sol.aux[mot] - 1; i++) {
        sol.mat_sol[mot][i] = sol.mat_sol[mot][i+1];
    }

    sol.aux[mot]--;
}