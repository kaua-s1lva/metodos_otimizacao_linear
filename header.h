#define MAX_MOC 10
#define MAX_OBJ 50

int num_obj;
int num_moc;
int vet_val_obj[MAX_OBJ];
int vet_pes_obj[MAX_OBJ];
int vet_cap_moc[MAX_MOC];

typedef struct tSolucao {
    int vet_obj[MAX_OBJ];
    int vet_pes_moc[MAX_OBJ];
    int fo;
} Solucao;

void ler_arquivo(char* path);
void imprimir_dados_arquivo();
void calcular_fo(Solucao& sol);
void gerar_solucao_aleatoria(Solucao& sol);
void gerar_solucao_gulosa(Solucao& sol);
void gerar_solucao_aleatoria_gulosa(Solucao& sol);
void gerar_vizinho(Solucao& sol);
void imprimir_solucao(Solucao& sol);
void heu_BL_rand(Solucao& sol, const int inter);
void heu_BL_MM(Solucao& sol);
void heu_BL_PM(Solucao& sol);