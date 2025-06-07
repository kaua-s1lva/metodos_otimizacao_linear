#define MAX_NOS 200

typedef struct tSolucao {
    int vet_hub[MAX_NOS];
    //float mat_ori_hub[MAX_NOS][MAX_NOS];
    //float mat_hub_hub[MAX_NOS][MAX_NOS];
    //float mat_hub_des[MAX_NOS][MAX_NOS];
    float mat_sol[MAX_NOS * MAX_NOS][5];
    //int mat_aux[MAX_NOS][MAX_NOS];
    float fo;
} Solucao;

int num_nos;
int num_hub;
//float x[MAX_NOS];
//float y[MAX_NOS];
float mat_dis[MAX_NOS][MAX_NOS];

void ler_arquivo(char* path);
void imprimir_dados_arquivo();
void gerar_solucao(Solucao& sol);
void calcular_solucao(Solucao& sol);
void imprimir_solucao(Solucao& sol);