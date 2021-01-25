//#include <stdio.h> // printf
#include "algoritmo.h"
#include "funcao.h"

// ALGORITMO TREPA-COLINAS -- PRONTO

float calcula_fit(int sol[], float **mat, int num_elem_conj, int num, struct info d) {
    int i, j;

    float sum_weight=0, sum_profit=0;

    for (i=0; i<num_elem_conj; i++) {
        if (sol[i] == 1) {
            for (j=i+1; j<num_elem_conj; j++)
            
                if (sol[j] == 1)
					// Actualiza o peso total
					sum_weight += mat[num][i + 1];
					sum_profit += mat[i][0];
		}
	}
	if (sum_weight > d.valorRes[num])
	{
		// Solucao inv�lida
		return 0;
	}
	else
	{

		// Solucao v�lida
		return sum_profit;
	}
}

// ALGORITMO EVOLUTIVO

// Calcula a qualidade de uma solu��o (dist�ncia m�dia)
// Param�tros de entrada:   solu��o (sol), matriz com as distancias (mat)
// Param�tros de sa�da:     qualidade da solu��o (dist�ncia m�dia)
float eval_individual(int sol[], struct info d, int mat[][10], int *v, int num)
{
	int     i;
	float   sum_weight, sum_profit;

	sum_weight = sum_profit = 0;
	// Percorre todos os objectos
	for (i = 0; i < d.popsize; i++)
	{
		// Verifica se o objecto i esta na mochila
		if (sol[i] == 1)
		{
			// Actualiza o peso total
			sum_weight += mat[num][i + 1];
			sum_profit += mat[i][0];
		}
	}
	if (sum_weight > d.valorRes[num])
	{
		// Solucao inv�lida
		*v = 0;
		return 0;
	}
	else
	{

		// Solucao v�lida
		*v = 1;
		return sum_profit;
	}
}

// Avalicao da populacao (fitness)
// Par�metros de entrada:   populacao (pop), estrutura com parametros (d) e matriz com as dist�ncias (mat)
void evaluate(pchrom pop, struct info d, int mat, num) {
    int i;

    for (i=0; i<d.popsize; i++) {
        pop[i].fitness = eval_individual(pop[i].p, d, mat, &pop[i].valido, num);
        //pop[i].fitness = eval_individual(pop[i].p, d, mat, &pop[i].valido); - N�o aplicavel
        //printf("Fitness: %f\n", pop[i].fitness);
    }
}
