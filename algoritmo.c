#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"
// Trepa-Colinas com Recristaliza��o Probabilistica
//#define PROB 0.01
#define PROB 0.0005
// Trepa-Colinas com Recristaliza��o Simulada
#define TMAX 0.99
//#define TMIN 5
#define TMIN 0.0001

// ALGORITMO TREPA-COLINAS

// Trepa-colinas
// Parametros de entrada:   solu��o (sol), matriz distancias (mat)
float trepa_colinas(int sol[], int mat, int num_elem_conj, int num_iter,struct info d, int num)
{
    int *nova_sol, i;
    float custo, custo_viz;
    // Recristaliza��o
    int *best_sol;
    float t = TMAX, decrementoT, best_custo;

    // Recristaliza��o
    decrementoT = (TMAX - TMIN) / num_iter;     // Recristaliza��o Simulada
    best_sol = malloc(num_elem_conj * sizeof(int));

    nova_sol = malloc(num_elem_conj * sizeof(int));
    if (nova_sol == NULL || best_sol == NULL) {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    custo = calcula_fit(sol, mat, num_elem_conj, num, d);

    // Recristaliza��o - Guarda melhor solu��o
    // Preenche a melhor solu��o com a actual
    best_custo = custo;
    substitui(best_sol, sol, num_elem_conj);
    

                // Fist Choice >
                custo_viz = gera_vizinho_first(sol, nova_sol, mat, num_elem_conj);
                // Aceita vizinho se fitness aumentar (problema de maximiza��o >)
                if (custo_viz > custo) {
                    substitui(sol, nova_sol, num_elem_conj);
                    custo = custo_viz;
                }
               
        // Recristaliza��o Simulada
        t = t - decrementoT;
  
    // Recristaliza��o
    substitui(sol, best_sol, num_elem_conj);
    free(best_sol);

    free(nova_sol);
    return custo;
}

// Gera vizinhos alterando um elemento aleat�riamente
// Parametros de entrada:   solu��o actual (sol), vizinho (nova_sol) e n�mero de elementos (num_elem_conj)
// Parametros de sa�da:     custo da solu��o gerada
float gera_vizinho_first(int sol[], int nova_sol[], int mat, int num_elem_conj, int num, struct info d)
{
    int i, x;

    for (i=0; i<num_elem_conj; i++)
        nova_sol[i] = sol[i];

    x = random_l_h(0, num_elem_conj-1);
    nova_sol[x] = !nova_sol[x];

    return calcula_fit(sol, mat, num_elem_conj, num, d);
}

// Gera vizinhos alternando cada um dos elementos indivudualmente. Retorna o melhor dos vizinhos gerados
// Parametros de entrada:   solu��o actual (sol), vizinho (nova_sol) e n�mero de elementos (num_elem_conj)
// Parametros de sa�da:     custo da melhor solu��o dos vizinhos gerados
/*float gera_vizinho_best(int sol[], int nova_sol[], float **mat, int num_elem_conj) {
    int i, j;
    int *viz;
    float maior, fit_viz;

    viz = malloc(num_elem_conj * sizeof(int));
    if (viz == NULL) {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    for (i=0; i<num_elem_conj; i++) {
        // C�pia solu��o para nova solu��o (Zera nova solu��o)
        for (j=0; j<num_elem_conj; j++)
            viz[j] = sol[j];
        // Altera cada um dos elemento da solu��o
        viz[i] = !viz[i];

        // Calcula o fitness do vizinho
        fit_viz = calcula_fit(sol, mat, num_elem_conj, num, d);

        // Na primeira solu��o, o fitness maior � a da primeira solu��o
        if (i == 0) {
            maior = fit_viz;

            for (j=0; j<num_elem_conj; j++) {
                nova_sol[j] = viz[j];
            }
        }
        // Verifica qual o melhor vizinho dos gerados e vai copiado o melhor para a nova solu��o
        // N�o corre no primeiro elemento porque este ser� o melhor conhecido at� ao momento
        if (fit_viz > maior && i>0) {
            maior = fit_viz;
            // Copia o vizinho maior para a nova solu��o
            for (j=0; j<num_elem_conj; j++) {
                nova_sol[j] = viz[j];
            }
        }
    }

    free(viz);
    return maior;
}*/

// ALGORITMO EVOLUTIVO

// Prenche uma estrutura com os progenitores da pr�xima gera��o, de acordo com os resultados do torneio binario (tamanho do torneio: 2)
// Par�metros de entrada:   popula��o actual (pop), estrutura com par�metros (d) e a popula��o de pais a preencher
void tournament(pchrom pop, struct info d, pchrom parents)
{
    int i, x1, x2;

    // Realiza torneios popsize vezes
    for (i=0; i<d.popsize; i++) {
        x1 = random_l_h(0, d.popsize-1);
        do
            x2 = random_l_h(0, d.popsize-1);
        while (x1 == x2);

        // Problema de maximiza��o: >
        if (pop[x1].fitness > pop[x2].fitness){
            parents[i] = pop[x1];
        } else {
            parents[i] = pop[x2];
        }
    }
}

// Prenche uma estrutura com os progenitores da pr�xima gera��o, de acordo com os resultados do torneio com tsize elementos
// Par�metros de entrada:   popula��o actual (pop), estrutura com par�metros (d) e a popula��o de pais a preencher
void tournament_tsize(pchrom pop, struct info d, pchrom parents)
{
    int i, j, xn, maior, *x = NULL;

    x = malloc(d.tsize * sizeof(int));
    // Se n�o conseguir alocar mem�ria, envia mensagem e fecha programa
    if (x == NULL) {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    // Realiza torneios popsize vezes
    for (i=0; i<d.popsize; i++) {
        x[0] = random_l_h(0, d.popsize-1);
        for (j=1; j<d.tsize; j++) {
            do {
                xn = random_l_h(0, d.popsize-1);
            } while (existe_val_arr(xn, x, j)); // tam = j, porque valores seguintes ainda est�o com valores aleat�rios do malloc, alem disso otimiza algoritmo
            x[j] = xn;
        }

        for (j=1, maior=0; j<d.tsize; j++) {
            if (pop[x[j]].fitness > pop[x[maior]].fitness)
                maior = j;
        }
        parents[i] = pop[x[maior]];
    }

    free(x);
}

// Operadores geneticos a usar na gera��o dos filhos
// Par�metros de entrada:   estrutura com os pais (parentes), estrutura com par�metros (d), estrutura que guardar� os descendentes (offsrping) e
// e algortimos de recombina��o e muta��o a usar
void genetic_operators(pchrom parents, struct info d, pchrom offspring, int opcao)
{
            // Recombina��o Uniforme
            crossover_uniform(parents, d, offspring);
            // Muta��o bin�ria
            mutation(offspring, d);
}

// Preenche o vector descendentes com o resultado das opera��es de recombina��o
// Par�metros de entrada:   estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offsrping)
void crossover(pchrom parents, struct info d, pchrom offspring)
{
    int i, j, point;

    // NOTA: Vai recombinar duas solu��es seguidas,
    // por isso i vai andar de dois em dois
    for (i=0; i<d.popsize; i+=2) {
        // Se valor gerado aleat�rio for inferior � Probabilidade de Recombina��o
        if (rand_01() < d.pr) {
            // Gera ponto de corte aleat�rio
			point = random_l_h(0, d.numGenes - 1);

            // Troca genes at� ponto de corte entre pais
            // Exemplo: 111000 e 000111:
            //offspring[i] = 000000
            //offspring[i+1] = 111111
            for (j=0; j<point; j++) {
                offspring[i].p[j] = parents[i].p[j];
				offspring[i+1].p[j] = parents[i+1].p[j];
            }
            //offspring[i] = 000111
            //offspring[i+1] = 111000
            for (j=point; j<d.numGenes; j++) {
                offspring[i].p[j]= parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        } else {
			offspring[i] = parents[i];
			offspring[i+1] = parents[i+1];
        }
    }
}

// Recombina��o com dois pontos de corte
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void crossover_2(pchrom parents, struct info d, pchrom offspring)
{
	int i, j, point1, point2;

    for (i=0; i<d.popsize; i+=2)
	{
		if (rand_01() < d.pr)
		{
			// Se os dois pontos forem 0 a solu��o fica igual
			//point1 = random_l_h(0, d.numGenes-1); // Corre��o Erro: Se ambos est�o at� d.numGenes-1 o ponto de corte � o mesmo
			point1 = random_l_h(0, d.numGenes-2);
			// Algortimo s� altera at� ao �ltimo antes do ponto corte
			// logo o segundo ponto de corte ser� o �ltimo elemento
			point2 = random_l_h(point1, d.numGenes-1);

			for (j=0; j<point1; j++)
			{
				offspring[i].p[j] = parents[i].p[j];
				offspring[i+1].p[j] = parents[i+1].p[j];
			}
			for (j=point1; j<point2; j++)
			{
				offspring[i].p[j]= parents[i+1].p[j];
				offspring[i+1].p[j] = parents[i].p[j];
			}
			for (j=point2; j<d.numGenes; j++)
            {
				offspring[i].p[j] = parents[i].p[j];
				offspring[i+1].p[j] = parents[i+1].p[j];
			}
		}
		else
		{
			offspring[i] = parents[i];
			offspring[i+1] = parents[i+1];
		}
	}
}

// Recombina��o uniforme
// Para cada gene � sorteado o n�mero 0 ou 1
// Se for 1, o primeiro filho recebe o gene do primeiro pai e o segundo filho o gene do segundo pai
// Se for 0, o primeiro filho recebe o gene do segundo pai e o segundo filho o gene do primeiro pai;
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void crossover_uniform(pchrom parents, struct info d, pchrom offspring)
{
    int i, j, resultado;

	for (i=0; i<d.popsize; i+=2) {
        if (rand_01() < d.pr) {

            for (j=0; j<d.numGenes; j++) {
                if ((resultado = flip())) {
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i+1].p[j] = parents[i+1].p[j];
                } else {
                    offspring[i].p[j]= parents[i+1].p[j];
                    offspring[i+1].p[j] = parents[i].p[j];
                }
            }

        } else {
			offspring[i] = parents[i];
			offspring[i+1] = parents[i+1];
		}
	}
}

// Muta��o bin�ria com v�rios pontos de muta��o
// Troca genes consoantes Probabilidade da Taxa de Muta��o
// Par�metros de entrada:   estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutation(pchrom offspring, struct info d)
{
    int i, j;

    for (i=0; i<d.popsize; i++) {
      
        for (j=0; j<d.numGenes; j++)
            if (rand_01() < d.pm)
                offspring[i].p[j] = !(offspring[i].p[j]);
    }
}

// Muta��o por troca: Escolhe aleteatoriamente objectos (um no conjunto e outro fora) e troca-os
// Par�metros de entrada: estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutation_troca(pchrom offspring, struct info d)
{
    int i, gen1, gen2;

    for (i=0; i<d.popsize; i++) {
        do {
            gen1 = random_l_h(0, d.numGenes-1);
        } while (offspring[i].p[gen1] != 1);
        do {
            gen2 = random_l_h(0, d.numGenes-1);
        } while (offspring[i].p[gen2] != 0);
        offspring[i].p[gen1] = 0;
        offspring[i].p[gen2] = 1;
    }
}
