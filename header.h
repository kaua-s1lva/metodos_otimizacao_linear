
#define MAX_TAR 25
#define MAX_MOT 10

int num_tarefas;
int temp_norm_trab;
int temp_max_trab;
int vet_hora_ini[MAX_TAR];
int vet_hora_fim[MAX_TAR];
int num_motoristas;

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
void criar_solucao_aleatoria(Solucao& sol);
void criar_solucao_gulosa(Solucao& sol);
void criar_solucao_aleatoria_gulosa(Solucao& sol, int taxa);
void calcular_fo_solucao(Solucao& sol);
void gerar_vizinho(Solucao& sol);
void imprimir_solucao(Solucao& sol);
void inserir_tarefa(Solucao& sol, int& tarefa, int& mot);
int inserir_tarefa_(Solucao& sol, int& tarefa, int& mot);
void remover_tarefa(Solucao& sol, int& pos, int& mot);
int remover_tarefa_(Solucao& sol, int& pos, int& mot);
void ordenar_vet_horas();
void heu_BL_rand(Solucao& sol, const int inter);
void heu_BL_MM(Solucao& sol);
void heu_BL_PM(Solucao& sol);