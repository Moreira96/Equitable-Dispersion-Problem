#define MAX_OBJ 1000		// Numero maximo de elementos



// ALGORITMO TREPA-COLINAS
float trepa_colinas(int sol[], int mat, int num_elem_conj, int num_iter);
float gera_vizinho_first(int sol[], int nova_sol[], float **mat, int num_elem_conj);
float gera_vizinho_best(int sol[], int nova_sol[], float **mat, int num_elem_conj);

// ALGORITMO EVOLUTIVO
// Estrutura para guardar parametros
struct info
{
    // Tamanho da popula��o
    int     popsize;
    // Probabilidade de muta��o
    float   pm;
    // Probabilidade de recombina��o
    float   pr;
	// Constante para avalia��o com penaliza��o
	float   ro;
    // Tamanho do torneio para sele��o do pai da pr�xima gera��o
    int tsize;
    // N�mero de elementos (N�mero de objetos que se podem colocar na mochila)
    int numGenes;
    // N�mero de gera��es
    int numGenerations;
	//Qualidade da solu��o �ptima
	float qualidade;
	// numero de restri��es
	int res;
	// valor das restri��es;
	int valorRes[10];
};

// Individuo (solu��o)
typedef struct individual chrom, *pchrom;

struct individual
{
    // Solu��o (objectos que est�o no conjunto)
    int p[MAX_OBJ]; //APAGAR<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Valor da qualidade da solu��o
    float fitness;
    // 1 se for solu��o v�lida e 0 se n�o for - N�o aplicavel
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
