#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)

#define PESO 1000

int main() {
    //srand(time(NULL));
    Solucao sol, sol2;

    ler_arquivo("../instancia-toy.txt");
    //imprimir_dados_arquivo();
/*
    for (int i=1; i<=num_cli; i++) {
        printf(" %d ", vet_dem_cli[i]);
    }
*/
//imprimir_dados_arquivo();
    gerar_solucao_gulosa(sol);
    calcular_fo(sol);
    imprimir_solucao(sol);
    heu_BL_PM(sol);
    calcular_fo(sol);
    imprimir_solucao(sol);
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
    for (int i = 1; i <= num_cli; i++) {
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
    for (int i = 1; i <= num_cli; i++) {
        vei = rand() % num_vei;
        sol.mat_sol[vei][sol.vet_aux[vei]] = i;

        sol.vet_aux[vei]++;
    }
}

void ordenar_clientes_por_demanda(int* vet_ord_cli) {
    int vet_cop_dem_cli[MAX_CLI];

    memset(vet_ord_cli, 0, (num_cli + 1) * sizeof(int));
    memcpy(&vet_cop_dem_cli, &vet_dem_cli, sizeof(vet_dem_cli));

    for (int i = 1; i <= num_cli; i++) {
        for (int j = 1; j <= num_cli; j++) {
            if (vet_cop_dem_cli[vet_ord_cli[i - 1]] < vet_cop_dem_cli[j] && vet_cop_dem_cli[vet_ord_cli[i]] != -1) {
                vet_ord_cli[i - 1] = j;
            }
        }
        vet_cop_dem_cli[vet_ord_cli[i - 1]] = -1;
    }
}

void gerar_solucao_gulosa(Solucao& sol) {

    //gerar solução com base na capacidade dos veículos
    int vet_dem_vei[MAX_VEI], vet_ord_cli[MAX_CLI], vei;
    bool passou = false;
    memset(&vet_dem_vei, 0, sizeof(vet_dem_vei));
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));

    ordenar_clientes_por_demanda(vet_ord_cli);

    for (int i = 1; i <= num_cli; i++) {
        passou = false;
        for (int j = 0; j < num_vei; j++) {
            if (vet_dem_vei[j] + vet_dem_cli[vet_ord_cli[i - 1]] <= vet_cap_vei[j]) {
                sol.mat_sol[j][sol.vet_aux[j]] = vet_ord_cli[i - 1];
                sol.vet_aux[j]++;
                vet_dem_vei[j] += vet_dem_cli[vet_ord_cli[i - 1]];
                passou = true;
                break;
            }
        }
        if (!passou) {
            vei = rand() % num_vei;
            sol.mat_sol[vei][sol.vet_aux[vei]] = vet_ord_cli[i - 1];
            sol.vet_aux[vei]++;
            vet_dem_vei[vei] += vet_dem_cli[vet_ord_cli[i - 1]];
        }
    }
}

void gerar_solucao_aleatoria_gulosa(Solucao& sol) {

    //gerar solução com base na capacidade dos veículos
    int vet_dem_vei[MAX_VEI], vet_ord_cli[MAX_CLI], vei;
    bool passou = false;
    memset(&vet_dem_vei, 0, sizeof(vet_dem_vei));
    memset(&sol.vet_aux, 0, sizeof(sol.vet_aux));
    memset(&sol.mat_sol, 0, sizeof(sol.mat_sol));

    ordenar_clientes_por_demanda(vet_ord_cli);

    for (int i = 1; i <= num_cli; i++) {
        passou = false;
        for (int j = 0; j < num_vei; j++) {
            if (vet_dem_vei[j] + vet_dem_cli[vet_ord_cli[i - 1]] <= vet_cap_vei[j]) {
                sol.mat_sol[j][sol.vet_aux[j]] = vet_ord_cli[i - 1];
                sol.vet_aux[j]++;
                vet_dem_vei[j] += vet_dem_cli[vet_ord_cli[i - 1]];
                passou = true;
                break;
            }
        }
        if (!passou) {
            vei = rand() % num_vei;
            sol.mat_sol[vei][sol.vet_aux[vei]] = vet_ord_cli[i - 1];
            sol.vet_aux[vei]++;
            vet_dem_vei[vei] += vet_dem_cli[vet_ord_cli[i - 1]];
        }
    }
}

void heu_BL_rand(Solucao& sol, int const& inter) {
    Solucao v;
    int flag = 1;

    while (flag) {
        flag = 0;
        for (int i = 0; i < inter; i++) {
            memcpy(&v, &sol, sizeof(sol));
            gerar_vizinho(v);
            calcular_fo(v);
            if (sol.fo > v.fo) {
                memcpy(&sol, &v, sizeof(v));
                flag = 1;
            }
        }
    }

}

void heu_BL_MM(Solucao& sol) {
    Solucao v;
    int flag = 1, cliente;

    memcpy(&v, &sol, sizeof(sol));

    while (flag) {
        flag = 0;
        for (int i = 0; i < num_vei; i++) {
            for (int j = 0; j < sol.vet_aux[i]; j++) {
                cliente = remover_cliente_do_veiculo_(sol, i, j);
                for (int k = 0; k < num_vei; k++) {
                    for (int l = 0; l < sol.vet_aux[k]; l++) {
                        inserir_cliente_no_veiculo(sol, k, l, cliente);
                        calcular_fo(sol);
                        if (v.fo > sol.fo) {
                            memcpy(&v, &sol, sizeof(sol));
                            flag = 1;
                        }
                        remover_cliente_do_veiculo(sol, k, l);
                    }
                }
                inserir_cliente_no_veiculo(sol, i, j, cliente);
            }
        }
        if (flag) {
            memcpy(&sol, &v, sizeof(v));
        }
    }
}

void heu_BL_PM(Solucao& sol) {
    int flag = 1, cliente, mel_sol = sol.fo;

    INICIO:;
        flag = 0;
        for (int i = 0; i < num_vei; i++) {
            for (int j = 0; j < sol.vet_aux[i]; j++) {
                cliente = remover_cliente_do_veiculo_(sol, i, j);
                for (int k = 0; k < num_vei; k++) {
                    for (int l = 0; l < sol.vet_aux[k]; l++) {
                        inserir_cliente_no_veiculo(sol, k, l, cliente);
                        calcular_fo(sol);
                        if (mel_sol > sol.fo) {
                            mel_sol = sol.fo;
                            goto INICIO;
                        }
                        remover_cliente_do_veiculo(sol, k, l);
                    }
                }
                inserir_cliente_no_veiculo(sol, i, j, cliente);
            }
        }
}

void calcular_fo(Solucao& sol) {
    sol.fo = 0;

    //custo do depósito até o primeiro cliente
    for (int i = 0; i < num_vei; i++) {
        sol.fo += mat_dis_cli[0][sol.mat_sol[i][0]];
    }

    //custo entre os clientes e o trajeto final até o depósito
    int capacidade;
    for (int i = 0; i < num_vei; i++) {
        capacidade = 0;
        for (int j = 0; j < sol.vet_aux[i]; j++) {

            capacidade += vet_dem_cli[sol.mat_sol[i][j]];

            if (capacidade > vet_cap_vei[i]) {
                sol.fo += PESO * mat_dis_cli[sol.mat_sol[i][j]][sol.mat_sol[i][j + 1]];
            }
            else {
                sol.fo += mat_dis_cli[sol.mat_sol[i][j]][sol.mat_sol[i][j + 1]];
            }

        }
    }
}

void gerar_vizinho(Solucao& sol) {
    int vei_pre, vei_pos, pos_cliente, cliente, vei_usados = 0, pos_cliente_pos;
    for (int i = 0; i < num_vei; i++) {
        if (sol.vet_aux[i] != 0) {
            vei_usados++;
        }
    }

    vei_pre = rand() % vei_usados;

    pos_cliente = rand() % sol.vet_aux[vei_pre];
    cliente = remover_cliente_do_veiculo_(sol, vei_pre, pos_cliente);

    do {
        vei_pos = rand() % num_vei;
        pos_cliente_pos = rand() % (sol.vet_aux[vei_pos] + 1);
    } while (vei_pre == vei_pos && pos_cliente_pos == pos_cliente);

    inserir_cliente_no_veiculo(sol, vei_pos, pos_cliente_pos, cliente);
}

void remover_cliente_do_veiculo(Solucao& sol, int veiculo, int posicao) {
    sol.mat_sol[veiculo][posicao] = 0;
    while (sol.mat_sol[veiculo][posicao + 1] != 0) {
        sol.mat_sol[veiculo][posicao] = sol.mat_sol[veiculo][posicao + 1];
        sol.mat_sol[veiculo][posicao + 1] = 0;
        posicao++;
    }
    sol.vet_aux[veiculo]--;
}

int remover_cliente_do_veiculo_(Solucao& sol, int veiculo, int posicao) {
    int cliente = sol.mat_sol[veiculo][posicao];

    sol.mat_sol[veiculo][posicao] = 0;
    while (sol.mat_sol[veiculo][posicao + 1] != 0) {
        sol.mat_sol[veiculo][posicao] = sol.mat_sol[veiculo][posicao + 1];
        sol.mat_sol[veiculo][posicao + 1] = 0;
        posicao++;
    }
    sol.vet_aux[veiculo]--;

    return cliente;
}

void inserir_cliente_no_veiculo(Solucao& sol, int veiculo, int posicao, int cliente) {
    for (int i = sol.vet_aux[veiculo]; i >= posicao; i--) {
        sol.mat_sol[veiculo][i + 1] = sol.mat_sol[veiculo][i];
    }
    sol.mat_sol[veiculo][posicao] = cliente;
    sol.vet_aux[veiculo]++;
}

void ler_arquivo(const char* path) {
    FILE* f = fopen(path, "r");

    int x[MAX_CLI], y[MAX_CLI];

    fscanf(f, "%d %d", &num_cli, &num_vei);

    for (int i = 0; i <= num_cli; i++) {
        fscanf(f, "%d %d %d %d", &x[i], &x[i], &y[i], &vet_dem_cli[i]);
    }

    for (int i = 0; i < num_vei; i++) {
        fscanf(f, "%d", &vet_cap_vei[i]);
    }

    for (int i = 0; i <= num_cli; i++) {
        for (int j = 0; j <= num_cli; j++) {
            mat_dis_cli[i][j] = sqrt(pow((x[i] - x[j]), 2) + pow((y[i] - y[j]), 2));
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
    for (int i = 0; i < num_vei; i++) {
        for (int j = 0; j < sol.vet_aux[i]; j++) {
            printf("%3d ", sol.mat_sol[i][j]);
        }
        printf("\n");
    }

    printf("\nVetor auxiliar: \n");
    for (int i = 0; i < num_vei; i++) {
        printf("%d ", sol.vet_aux[i]);
    }
}

void imprimir_dados_arquivo() {
    printf("\n-------------------------------------------------------------\n");
    printf("\nDados do arquivo: \n");
    printf("Clientes: %d      Veiculos: %d\n", num_cli, num_vei);

    printf("\nMatriz das distancias: \n");
    for (int i = 0; i <= num_cli; i++) {
        for (int j = 0; j <= num_cli; j++) {
            printf("%8.2f ", mat_dis_cli[i][j]);
        }
        printf("\n");
    }

    printf("\nVetor de capacidade dos veiculos:\n");
    for (int i = 0; i < num_vei; i++) {
        printf("%d ", vet_cap_vei[i]);
    }

    printf("\nVetor de demanda dos clientes: \n");
    for (int i = 0; i <= num_cli; i++) {
        printf("%d ", vet_dem_cli[i]);
    }
    printf("\n-------------------------------------------------------------\n");
}