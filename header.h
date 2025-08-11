#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

#define MAX_PTS 14000 // maximo de pontos a serem rotulados

typedef struct tSolucao
{
    int vet_sol[MAX_PTS];
    int fo;
}Solucao;

int num_pts;         // numero de pontos
int num_pos;         // numero de posicoes candidatas por ponto
int num_ids;         // numero total (num_pts * num_pos) de identificadores (posicoes possiveis)
int* vet_qtd_conf;   // vetor com a quantidade de conflitos de cada identificador
int** mat_conflitos; // matriz com o "id" dos conflitos de cada identificador
//----------------------------------------------------------------------------------------------------------

void gerar_vizinha(Solucao& s);
void heu_const_ale(Solucao& s);
void heu_const_gul(Solucao& s);
void heu_const_ale_gul(Solucao& s);

void heu_BL_rand(Solucao& s, const int& iter);
void heu_BL_MM(Solucao& s);
void heu_BL_PM(Solucao& s);

//----------------------------------------------------------------------------------------------------------
void calcular_fo(Solucao& s);
int get_ponto(const int& id);
int get_posicao(const int& id);
int get_id(const int& pt, const int& pos);
void escrever_solucao(const Solucao& s, char* arq);
void testar_dados(char* arq);
void ler_dados(char* arq);

#endif