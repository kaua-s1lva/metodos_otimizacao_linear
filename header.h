#define MAX_CLI 200
#define MAX_VEI 20

typedef struct tSolucao {
    int mat_sol[MAX_VEI][MAX_CLI];
    int vet_aux[MAX_VEI];
    float fo;
} Solucao;

int num_cli;
int num_vei;
float mat_dis_cli[MAX_CLI][MAX_CLI];
int vet_cap_vei[MAX_VEI];
int vet_dem_cli[MAX_CLI];

void ler_arquivo(const char* path);
void imprimir_dados_arquivo();
void gerar_solucao(Solucao& sol);
void gerar_solucao_aleatoria(Solucao& sol);
void gerar_solucao_gulosa(Solucao& sol);
void gerar_solucao_aleatoria_gulosa(Solucao& sol);
void calcular_fo(Solucao& sol);
void imprimir_solucao(Solucao& sol);
void gerar_vizinho(Solucao& sol);
void ordenar_clientes_por_demanda(int* vet_ord_cli);
void heu_BL_rand(Solucao& sol, int const& inter);
void heu_BL_MM(Solucao& sol);
void heu_BL_PM(Solucao& sol);
void remover_cliente_do_veiculo(Solucao& sol, int veiculo, int posicao);
void inserir_cliente_no_veiculo(Solucao& sol, int veiculo, int posicao, int cliente);
int remover_cliente_do_veiculo_(Solucao& sol, int veiculo, int posicao);