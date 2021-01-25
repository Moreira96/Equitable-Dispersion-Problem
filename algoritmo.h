#define MAX_OBJ 1000		// Numero maximo de elementos



// ALGORITMO TREPA-COLINAS
float trepa_colinas(int sol[], int mat, int num_elem_conj, int num_iter);
float gera_vizinho_first(int sol[], int nova_sol[], float **mat, int num_elem_conj);
float gera_vizinho_best(int sol[], int nova_sol[], float **mat, int num_elem_conj);

// ALGORITMO EVOLUTIVO
// Estrutura para guardar parametros
struct info
{
    // Tamanho da população
    int     popsize;
    // Probabilidade de mutação
    float   pm;
    // Probabilidade de recombinação
    float   pr;
	// Constante para avaliação com penalização
	float   ro;
    // Tamanho do torneio para seleção do pai da próxima geração
    int tsize;
    // Número de elementos (Número de objetos que se podem colocar na mochila)
    int numGenes;
    // Número de gerações
    int numGenerations;
	//Qualidade da solução óptima
	float qualidade;
	// numero de restrições
	int res;
	// valor das restrições;
	int valorRes[10];
};

// Individuo (solução)
typedef struct individual chrom, *pchrom;

struct individual
{
    // Solução (objectos que estão no conjunto)
    int p[MAX_OBJ]; //APAGAR<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Valor da qualidade da solução
    float fitness;
    // 1 se for solução válida e 0 se não for - Não aplicavel
    //int valido;
};
//
void tournament(pchrom pop, struct info d, pchrom parents);
void tournament_tsize(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring, int opcao);
void crossover(pchrom parents, struct info d, pchrom offspring);
void crossover_2(pchrom parents, struct info d, pchrom offspring);
void crossover_uniform(pchrom parents, struct info d, pchrom offspring);
void mutation(pchrom offspring, struct info d);
void mutation_troca(pchrom offspring,struct info d);
