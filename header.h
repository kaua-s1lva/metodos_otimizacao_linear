
#define MAX_TAR 26
#define MAX_MOT 12 //IMPORTANTE! EXISTE UM CÁLCULO MINIMO, SERÁ NECESSÁRIO FAZER ESSE CÁLCULO

int num_tarefas;
int temp_norm_trab;
int temp_max_trab;
int vet_hora_ini[MAX_TAR];
int vet_hora_fim[MAX_TAR];

typedef struct tSolucao {
    int hora_extra;
    int temp_ocios;
    int temp_sobre;
    int temp_exces;
    int mat_sol[MAX_MOT][MAX_TAR];
    int aux[MAX_MOT];
    int vet_hora_trab[MAX_TAR];
    int fo;
} Solucao;

void ler_arquivo(char* path);
void imprimir_dados_arquivo();
void criar_solucao(Solucao& sol);
void calcular_fo_solucao(Solucao& sol);
void gerar_vizinho(Solucao& sol);
void imprimir_solucao(Solucao& sol);
void inserir_tarefa(Solucao& sol, int& tarefa, int& mot);
void remover_tarefa(Solucao& sol, int& pos, int& mot);