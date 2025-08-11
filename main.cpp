#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "header.h"

int main()
{
    Solucao s;
    ler_dados("../i25.txt");
    //testar_dados("");
    //---

    //srand(time(NULL));

    heu_const_ale(s);
    calcular_fo(s);
    escrever_solucao(s, "");

    //heu_BL_rand(s, 3 * (num_pos + 1) * num_pts);
    heu_BL_PM(s);
    escrever_solucao(s, "");
    //---
    delete[] vet_qtd_conf;
    for(int i = 0; i < num_ids; i++)
        delete[] mat_conflitos[i];
    delete[] mat_conflitos;
    return 0;
}

//----------------------------------------------------------------------------------------------------------

void gerar_vizinha(Solucao& s)
{
    int pt, pos;
    pt = rand() % num_pts;
    do
        pos = rand() % num_pos;
    while (pos == s.vet_sol[pt]);
    s.vet_sol[pt] = pos;
}

void heu_const_ale(Solucao& s)
{
    for (int i = 0; i < num_pts; i++)
        s.vet_sol[i] = rand() % num_pos;
}

void heu_const_gul(Solucao& s)
{
    memset(&s.vet_sol, -1, sizeof(s.vet_sol));
    for (int i = 0; i < num_pts; i++)
    {
        int melhor_pos;
        int melhor_id = -1;
        int min_conflitos = num_pts;
        for (int j = 0; j < num_pos; j++)
        {
            int id = get_id(i, j);
            int conflitos = 0;
            for (int k = 0; k < vet_qtd_conf[id]; k++)
            {
                int pt = get_ponto(mat_conflitos[id][k]);
                int pos = get_posicao(mat_conflitos[id][k]);
                if (s.vet_sol[pt] == pos)
                    conflitos++;
            }
            if ((conflitos < min_conflitos) || ((conflitos == min_conflitos) && vet_qtd_conf[id] < vet_qtd_conf[melhor_id]))
            {
                min_conflitos = conflitos;
                melhor_id = id;
                melhor_pos = j;
            }
        }
        s.vet_sol[i] = melhor_pos;
    }
}

void heu_const_ale_gul(Solucao& s)
{
    int vet_aux[MAX_PTS];
    for (int i = 0; i < num_pts; i++)
        vet_aux[i] = i;
    memset(&s.vet_sol, -1, sizeof(s.vet_sol));
    for (int i = 0; i < num_pts; i++)
    {
        int pos_sel = i + rand() % (num_pts - i);
        int melhor_pos;
        int melhor_id = -1;
        int min_conflitos = num_pts;
        for (int j = 0; j < num_pos; j++)
        {
            int id = get_id(vet_aux[pos_sel], j);
            int conflitos = 0;
            for (int k = 0; k < vet_qtd_conf[id]; k++)
            {
                int pt = get_ponto(mat_conflitos[id][k]);
                int pos = get_posicao(mat_conflitos[id][k]);
                if (s.vet_sol[pt] == pos)
                    conflitos++;
            }
            if ((conflitos < min_conflitos) || ((conflitos == min_conflitos) && vet_qtd_conf[id] < vet_qtd_conf[melhor_id]))
            {
                min_conflitos = conflitos;
                melhor_id = id;
                melhor_pos = j;
            }
        }
        s.vet_sol[vet_aux[pos_sel]] = melhor_pos;
        int aux = vet_aux[pos_sel];
        vet_aux[pos_sel] = vet_aux[i];
        vet_aux[i] = aux;
    }
}

//----------------------------------------------------------------------------------------------------------

void heu_BL_rand(Solucao& s, const int& iter)
{
    Solucao v;
    int flag = 1;
    
    while (flag) {
        flag = 0;
        for (int i=0; i<iter; i++) {
            memcpy(&v, &s, sizeof(s));
            gerar_vizinha(v);
            calcular_fo(v);
            if (s.fo < v.fo) {
                memcpy(&s, &v, sizeof(v));
                flag = 1;
            }
        }
    }

    calcular_fo(s);
}

void heu_BL_MM(Solucao& s)
{
    int flag = 1, pos, mel_fo = s.fo, mel_pos, mel_pnt;
    while (flag) {
        flag = 0;
        for (int i=0; i<num_pts; i++) {
            pos = s.vet_sol[i];
            for (int j=0; j<num_pos; j++) {
                s.vet_sol[i] = j;
                calcular_fo(s);
                if (mel_fo < s.fo) {
                    mel_fo = s.fo;
                    mel_pos = j;
                    mel_pnt = i;
                    flag = 1;
                }
            }
            s.vet_sol[i] = pos;
        }
        if (flag) {
            s.vet_sol[mel_pnt] = mel_pos;
            calcular_fo(s);
        }
    }

}

void heu_BL_PM(Solucao& s)
{
    int flag = 1, pos, mel_fo = s.fo, mel_pos, mel_pnt;
    
    INICIO : ;
        for (int i=0; i<num_pts; i++) {
            pos = s.vet_sol[i];
            for (int j=0; j<num_pos; j++) {
                s.vet_sol[i] = j;
                calcular_fo(s);
                if (mel_fo < s.fo) {
                    mel_fo = s.fo;
                    mel_pos = j;
                    mel_pnt = i;
                    goto INICIO;
                }
            }
            s.vet_sol[i] = pos;
        }
    calcular_fo(s);
}


void calcular_fo(Solucao& s)
{
    int id, pt, pos, conflito;
    s.fo = num_pts;                                  // considera que todos os pontos estao livres (sem conflito)
    for (int i = 0; i < num_pts; i++)                // para cada ponto
    {
        id = get_id(i, s.vet_sol[i]);                // pega o id da posicao candidata selecionada para o ponto na solucao
        conflito = 0;                                // considera que o ponto esta livre (nao esta em conflito)
        for (int j = 0; j < vet_qtd_conf[id]; j++)   // para cada possivel conflito do id (posicao selecionada do ponto)
        {
            pt = get_ponto(mat_conflitos[id][j]);    // pega o ponto do possivel conflito
            pos = get_posicao(mat_conflitos[id][j]); // pega a posicao candidata do possivel conflito
            if (s.vet_sol[pt] == pos)                // verifica se na solucao para o ponto foi selecionada a posicao candidata confitante
            {
                conflito = 1;                        // o ponto nao esta livre, ha pelo menos um conflito
                break;
            }
        }
        s.fo -= conflito;                            // atualiza a fo
    }
}

// recebe um ponto (0 a num_pts - 1) e uma posicao candidata (0 a num_pos - 1) e retorna o id (0 a num_ids - 1)
int get_id(const int& pt, const int& pos)
{
    return (pt * num_pos) + pos;
}

// recebe um id (0 a num_ids - 1) e retorna o ponto (0 a num_pts - 1)
int get_ponto(const int& id)
{
    return id / num_pos;
}

// recebe um id (0 a num_ids - 1) e retorna a posicao candidata (0 a num_pos - 1)
int get_posicao(const int& id)
{
    return id % num_pos;
}

void escrever_solucao(const Solucao& s, char* arq)
{
    FILE* f;
    if (arq == "")
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "\n\nFO: %d\n", s.fo);
    for (int i = 0; i < num_pts; i++)
        fprintf(f, "Ponto: %d\tPos: %d\n", i + 1, s.vet_sol[i] + 1); // os ids no arquivo de entrada comecam em 1 (ao inves de 0)
    if (arq != "")
        fclose(f);
}

void testar_dados(char* arq)
{
    FILE* f;
    if (arq == "")
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "%d\n%d", num_pts, num_pos);
    for(int i = 0; i < num_ids; i++)
    {
        fprintf(f, "\n%d\n", vet_qtd_conf[i]);
        for(int j = 0; j < vet_qtd_conf[i]; j++)
            fprintf(f, "%d ", mat_conflitos[i][j] + 1); // os ids no arquivo de entrada comecam em 1 (ao inves de 0)
    }
    if (arq != "")
        fclose(f);
}

void ler_dados(char* arq)
{
    FILE* f = fopen(arq, "r");
    fscanf(f, "%d %d", &num_pts, &num_pos);
    num_ids = num_pts * num_pos;
    vet_qtd_conf = new int[num_ids];
    mat_conflitos = new int*[num_ids];
    for(int i = 0; i < num_ids; i++)
    {
        fscanf(f, "%d", &vet_qtd_conf[i]);
        mat_conflitos[i] = new int[vet_qtd_conf[i]];
        for(int j = 0; j < vet_qtd_conf[i]; j++)
        {
            fscanf(f, "%d", &mat_conflitos[i][j]);
            mat_conflitos[i][j]--; // os ids no arquivo de entrada comecam em 1 (ao inves de 0)
        }
    }
    fclose(f);
}