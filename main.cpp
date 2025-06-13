#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    //srand(time(NULL));

    Solucao sol, sol2;

    ler_arquivo("../pmm1.txt");
    //imprimir_dados_arquivo();

    gerar_solucao_gulosa(sol);

    memcpy(&sol2, &sol, sizeof(sol2));
    gerar_vizinho(sol2);

    imprimir_solucao(sol);
    imprimir_solucao(sol2);
    
    return 0;
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

void imprimir_solucao(Solucao& sol) {
    printf("\n\nVetor solucao: \n");
    for (int i=0; i<num_obj; i++) {
        printf(" %d ", sol.vet_obj[i]);
    }
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