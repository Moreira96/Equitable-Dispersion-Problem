#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "utils.h"

// FUNCOES COMUNS ALGORITMOS

// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de elementos (n) (ptr)
// Devolve a matriz de adjacencias
struct info init_dados(char *filename, int mat[][10])
{
	struct  info x;
	FILE    *f;
	int     i, j;

	f = fopen(filename, "rt");
	if (!f)
	{
		printf("File not found\n");
		exit(1);
	}
	// Leitura dos parâmetros do problema
	// número de objetos (N), número de restrições (M) e qualidade da
	//solução ótima(caso seja conhecida);
	fscanf(f, "%d %d %f", &x.popsize, &x.res, &x.qualidade);
	x.numGenes = x.popsize;
	//------------------------------------------------------------------------------

	printf("%d %d %f\n", x.popsize, x.res, x.qualidade);
	//------------------------------------------------------------------------------
	// Valor de cada um dos N objetos; 
	for (i = 0; i < x.popsize; i++) {
		fscanf(f, "%d ", &mat[i][0]);
		printf("%d ", mat[i][0]);
	}
	printf("\n");
	for (i = 0; i < x.res; i++) {
		for (j = 1; j < x.popsize + 1; j++) {
			fscanf(f, " %d", &mat[i][j]);
			printf(" %d", mat[i][j]);
			if (j == x.popsize)
				printf("\n");
		}
	}
	for (i = 0; i < x.res; i++)
		fscanf(f, " %d", &x.valorRes[i]);
	for (i = 0; i < x.res; i++)
		printf("%d ", x.valorRes[i]);
	system("pause");

	if (x.numGenes > MAX_OBJ)
	{
		printf("Number of itens is superior to MAX_OBJ\n");
		exit(1);
	}
	x.ro = 0.0;
	fclose(f);
	// Devolve a estrutura com os parâmetros
	return x;
}

// gerador de numeros aleatorios
void init_rand()
{
	srand((unsigned)time(NULL));
}

// Devolve um valor inteiro distribuido uniformemente entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max-min+1);
}

// Retorna aleatóriamente valor 0 ou 1
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	else
		return 1;
}

// ALGORITMO TREPA-COLINAS

// Gera solução inicial
// Parametros de entrada:   solução (sol), número de vértices (v)
void gera_sol_inicial(int *sol, int v)
{
    int i;

    for (i=0; i<v; i++)
        sol[i] = flip();
}

// Escreve solução
// Parametros de entrada:   solução (sol), número de vértices (v)
void escreve_sol(int *sol, int num_elem_conj)
{
    int i, elementos;

    printf("\nSubconjunto: ");

    for (i=0; i<num_elem_conj; i++)
        if (sol[i] == 1)
            printf("%3d ", i+1);

    for (i=0, elementos=0; i<num_elem_conj; i++)
        if (sol[i])
            elementos++;
    printf("\nNumero de elementos: %d\n", elementos);
}

// Copia vector b para vector a
// Parametros de entrada:   vector origem (a), vector destino (b), tamanho do vector (n)
void substitui(int a[], int b[], int n) {
    int i;

    //printf("\n\nSubsitui solucao\n\n");
    for (i=0; i<n; i++) {
        a[i]=b[i];
    }
}

// ALGORTIMO EVOLUTIVO

// Criação da população inicial (vector alocado dinamicamente)
// Parâmetro de entrada:    Estrutura com parâmtros do problema (tamanho da população)
// Parâmetro de saída:      Preenche da estrutura da população apenas o vector binário com os elementos que estão dentro ou fora da mochila
pchrom init_pop(struct info d)
{
    int i, j;
    pchrom indiv;

    indiv = malloc(d.popsize * sizeof(chrom));
    if (indiv == NULL)
    {
        printf("Erro na alocacao de memoria\n");
		exit(1);
    }

    //printf("Populacao inicial:\n");
    for(i=0; i<d.popsize; i++) {
        for (j=0; j<d.numGenes; j++) {
            indiv[i].p[j] = flip();
        }
    }

    return indiv;
}

// Actualiza a melhor solução encontrada
// Parâmetro de entrada:    população actual (pop), estrutura com parâmetros (d) e a melhor solução encontrada até à geração imediatamente anterior (best)
// Parâmetro de saída:      a melhor solução encontrada até à geração actual
chrom get_best(pchrom pop, struct info d, chrom best)
{
    int i;

    for (i=0; i<d.popsize; i++) {
        if (best.fitness < pop[i].fitness)
            best = pop[i];
    }
    return best;
}

// Devolve um valor real entre 0 e 1
float rand_01()
{
	return ((float)rand())/RAND_MAX;
}

// Escreve uma solução na consola
// Parâmetros de entrada:   solução (chrom) e estrutura com parâmetros (d)
void write_best(chrom x, struct info d)
{
    int i, elementos;

    printf("\nBest individual: %4.3f\n", x.fitness);

    for (i=0, elementos=0; i<d.numGenes; i++)
        if (x.p[i])
            elementos++;
    printf("Numero de elementos: %d\n", elementos);
    for (i=0; i<d.numGenes; i++)
        printf("%d", x.p[i]);
    putchar('\n');
}

// Verifica se existe um valor numa matriz
// Parâmetros de entrada: valor a verificar (val), matriz (mat) e tamanho da matriz (tam)
int existe_val_arr(int val, int* mat, int tam)
{
    int i, existe = 0;

    for (i=0; i<tam; i++) {
		if (mat[i] == val) {
            existe = 1;
        }
    }
    return existe;
}
