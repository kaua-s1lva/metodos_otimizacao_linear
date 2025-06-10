#define MAX_NOS 50

typedef struct tSolucao {
    int vet_hub[MAX_NOS];
    float mat_sol[MAX_NOS * MAX_NOS][5];
    float fo;
} Solucao;

int num_nos;
int num_hub;
float mat_dis[MAX_NOS][MAX_NOS];

void ler_arquivo(char* path);
void imprimir_dados_arquivo();
void criar_solucao(Solucao& sol);
void criar_solucao_aleatoria(Solucao& sol);
void criar_solucao_gulosa(Solucao& sol);
void criar_solucao_aleatoria_gulosa(Solucao& sol);
void calcular_solucao(Solucao& sol);
void imprimir_solucao(Solucao& sol);
void gerar_vizinho(Solucao& sol);