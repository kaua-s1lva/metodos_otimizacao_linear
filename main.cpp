#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX(X,Y) (X > Y ? X : Y)

#define PESO_CAP 100

int main() {
    //srand(time(NULL));

    Solucao sol, sol2;

    ler_arquivo("../pmm1.txt");
    //imprimir_dados_arquivo();

    gerar_solucao_aleatoria(sol);
    calcular_fo(sol);
    imprimir_solucao(sol);

    heu_BL_PM(sol);
    calcular_fo(sol);

    imprimir_solucao(sol);
    
    return 0;
}

void calcular_fo(Solucao& sol)
{
    sol.fo = 0;
    memset(&sol.vet_pes_moc, 0, sizeof(sol.vet_pes_moc));
    for (int j = 0; j < num_obj; j++)
    {
        if (sol.vet_obj[j] != -1)
        {
            sol.fo += vet_val_obj[j];
            sol.vet_pes_moc[sol.vet_obj[j]] += vet_pes_obj[j];
        }
    }
    for (int i = 0; i < num_moc; i++)
        sol.fo -= PESO_CAP * MAX(0, (sol.vet_pes_moc[i] - vet_cap_moc[i]));
}

void gerar_vizinho(Solucao& sol) {
    int objeto, mochila;

    objeto = rand() % num_obj;
    do {
        mochila = (rand() % (num_moc + 1)) - 1;
    } while (mochila == sol.vet_obj[objeto]);

    sol.vet_obj[objeto] = mochila;
}

void gerar_solucao_aleatoria(Solucao& sol) {
    int mochila;
    for (int i=0; i<num_obj; i++) {
        mochila = (rand() % (num_moc + 1)) - 1;
        sol.vet_obj[i] = mochila;
    }
}

void gerar_solucao_gulosa(Solucao& sol) {
    int peso_atual_moc[MAX_MOC];
    bool verificador;
    memset(&peso_atual_moc, 0, sizeof(peso_atual_moc));

    for (int i=0; i<num_obj; i++) {
        verificador = false;
        for (int j=0; j<num_moc; j++) {
            if (peso_atual_moc[j] + vet_pes_obj[i] <= vet_cap_moc[j]) {
                sol.vet_obj[i] = j;
                peso_atual_moc[j] += vet_pes_obj[i];
                verificador = true;
                break;
            }
        }
        if (!verificador) {
            sol.vet_obj[i] = -1;
        }
    }
}

void gerar_solucao_aleatoria_gulosa(Solucao& sol) {
    int peso_atual_moc[MAX_MOC];
    bool verificador;
    memset(&peso_atual_moc, 0, sizeof(peso_atual_moc));

    for (int i=0; i<num_obj; i++) {
        verificador = false;
        for (int j=0; j<num_moc; j++) {
            if (peso_atual_moc[j] + vet_pes_obj[i] <= vet_cap_moc[j]) {
                sol.vet_obj[i] = j;
                peso_atual_moc[j] += vet_pes_obj[i];
                verificador = true;
                break;
            }
        }
        if (!verificador) {
            sol.vet_obj[i] = (rand() % (num_moc + 1)) - 1;
        }
    }
}

void heu_BL_rand(Solucao& sol, const int inter) {
    Solucao vizinho;
    int flag = 1;

    while (flag)
    {
        flag = 0;
        for (int i=0; i<inter; i++) {
            memcpy(&vizinho, &sol, sizeof(vizinho));
            gerar_vizinho(vizinho);
            calcular_fo(vizinho);
            if (sol.fo <= vizinho.fo) {
                memcpy(&sol, &vizinho, sizeof(sol));
                flag = 1;
            }
        }
    }
}

void heu_BL_MM(Solucao& sol) {
    int moc_ori, mel_sol = sol.fo, flag = 1;

    while (flag) {
        flag = 0;
        for (int i=0; i<num_obj; i++) {
            for (int j=-1; j<num_moc; j++) {
                moc_ori = sol.vet_obj[i];
                sol.vet_obj[i] = j;
                calcular_fo(sol);
                if (mel_sol < sol.fo) {
                    mel_sol = sol.fo;
                    flag = 1;
                } else {
                    sol.vet_obj[i] = moc_ori;
                }
            }
        }
    }
}

void heu_BL_PM(Solucao& sol) {
    int moc_ori, mel_sol = sol.fo, flag = 1;

    while (flag) {
        flag = 0;
        INICIO : ;
        for (int i=0; i<num_obj; i++) {
            for (int j=-1; j<num_moc; j++) {
                moc_ori = sol.vet_obj[i];
                sol.vet_obj[i] = j;
                calcular_fo(sol);
                if (mel_sol < sol.fo) {
                    mel_sol = sol.fo;
                    flag = 1;
                    goto INICIO;
                } else {
                    sol.vet_obj[i] = moc_ori;
                }
            }
        }
    }
}

void imprimir_solucao(Solucao& sol) {
    printf("\n\nVetor solucao: \n");
    for (int i=0; i<num_obj; i++) {
        printf(" %d ", sol.vet_obj[i]);
    }
    printf("\nFO: %d", sol.fo);
}

void ler_arquivo(char* path) {
    FILE* f = fopen(path, "r");

    fscanf(f, "%d %d", &num_obj, &num_moc);
    for (int i=0; i<num_obj; i++) {
        fscanf(f, "%d", &vet_val_obj[i]);
    }

    for (int i=0; i<num_obj; i++) {
        fscanf(f, "%d", &vet_pes_obj[i]);
    }

    for (int i=0; i<num_moc; i++) {
        fscanf(f, "%d", &vet_cap_moc[i]);
    }

    fclose(f);
}

void imprimir_dados_arquivo() {
    printf("%d %d\n", num_obj, num_moc);
    for (int i=0; i<num_obj; i++) {
        printf("%d ", vet_val_obj[i]);
    }
    printf("\n");

    for (int i=0; i<num_obj; i++) {
        printf("%d ", vet_pes_obj[i]);
    }
    printf("\n");

    for (int i=0; i<num_moc; i++) {
        printf("%d ", vet_cap_moc[i]);
    }
}