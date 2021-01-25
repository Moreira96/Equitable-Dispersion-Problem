#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // strcpy
#include <windows.h>    // Beep

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS 10

int main(int argc, char *argv[])
{
	int ap = 0;
    char nome_fich[100];
    int runs, opcao, opcao2, num_elem_conj, i, r, mat[MAX_OBJ][10], num = 0;
	struct info param;

    // L� o nome do ficheiro e o n�mero de repeti��es apartir dos parametros passados
    // Se n�o for passado o n�mero de itera��es, assume o valor de DEFAULT_RUNS
    // Se n�o for passado nenhum parametro, pergunta o nome do ficheiro
    if(argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else
        if(argc == 2)
        {
            runs = DEFAULT_RUNS;
            strcpy(nome_fich, argv[1]);
        }
        else
        {
            runs = DEFAULT_RUNS;
            printf("Nome do Ficheiro: ");
            gets(nome_fich);
        }
    // Termina programa se n�mero itera��es for igual ou inferior a zero
    if(runs <= 0)
        return 0;

    // Inicializa o gerador de numeros aleatorios
    init_rand();

    // Carrega valores para matriz
    param = init_dados(nome_fich, mat);
	num_elem_conj = param.res;
    // Escolha algoritmo a utilizar
    puts("Introduza o algoritmo a usar:");
    puts("1 - Trepa-Colinas First Choice");
    puts("2 - Evolutivo");
    puts("3 - Metodo Hibrido");
    puts("4 - Sair\n");
    do {
        printf("Opcao: ");
        scanf(" %d", &opcao);
        printf("%d", opcao);
    } while (opcao < 1 || opcao > 4);

    switch (opcao) {
        case 1:
		{
			// Pesquisa Local - Trepa Colinas
			int num_iter;
			float custo, best_custo;
			int *sol, *best;
			float mbf = 0.0;

			num_iter = 100;

				sol = malloc(sizeof(int)*num_elem_conj); // Alocar espa�o para a solu��o 
				best = malloc(sizeof(int)*num_elem_conj); // Alocar espa�o para guardar a melhor solu��o
				if (sol == NULL || best == NULL)
				{
					printf("Erro na alocacao de memoria");
					exit(1);
				}

				putchar('\n');
				for (r = 0; r < runs; r++)
				{
					// Gera solu��o inicial
					gera_sol_inicial(sol, num_elem_conj);
					// Escreve solu��o inicial
					escreve_sol(sol, num_elem_conj);
					// Trepa colinas
					custo = trepa_colinas(sol, mat, num_elem_conj, num_iter, num, param);
					// Escreve resultados da repeti��o r
					printf("\nRepeticao %d: ", r + 1);
					escreve_sol(sol, num_elem_conj);
					printf("Custo final: %.3f\n", custo);
					mbf += custo;
					// Problema de maximiza��o
					if (r == 0 || custo > best_custo) {
						best_custo = custo;
						substitui(best, sol, num_elem_conj);
					}
				}
				// Escreve resultados globais
				printf("\nMBF: %.3f\n", mbf / r);
				printf("\nMelhor solucao encontrada\t\a\7");
				escreve_sol(best, num_elem_conj);
				printf("Custo final: %.3f\n", best_custo);
				free(sol);
				free(best);
				break;
			}
			
        case 2:
        {
            // Algoritmo Evolutivo
            struct info param;
            pchrom      pop = NULL, parents = NULL;
            chrom       best_run, best_ever;
            int         gen_actual;
            float       mbf = 0.0;

            // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input - n�o aplicavel
            //EA_param = init_data(nome_fich, mat);
            param.popsize = 100;              // Tamanho da popula��o
            param.pr = 1.0;                 // Probabilidade de recombina��o
            param.pm = 0.01;                 // Probabilidade de muta��o
            param.tsize = 2;                // Tamanho do torneio para sele��o do pai da pr�xima gera��o
            param.numGenerations = 2500;       // N�mero de gera��es
            param.numGenes = num_elem_conj; // N�mero de elementos (N�mero de objetos que se podem colocar na mochila)

            if (param.numGenes > MAX_OBJ) {
                printf("N�mero de elementos superior ao MAX_ELE\n");
                exit(1);
            }

            //param.ro = 0.0;

            // Leitura dos dados do KSP (peso e lucro) - n�o aplicavel
            //for (i=0; i<x.numGenes; i++)
            //  fscanf(f, " %d %d", &mat[i][0], &mat[i][1]);

            // Escolha algoritmo a utilizar
            printf("\n\n");
            puts("Introduza os algoritmos de recombinacao e mutacao a usar:");
            puts("1 - Recombinacao 1 Ponto Corte e Mutacao Binaria");
            puts("2 - Recombinacao 1 Ponto Corte e Mutacao por Troca");
            puts("3 - Recombinacao 2 Pontos Corte e Mutacao Binaria");
            puts("4 - Recombinacao Uniforme e Mutacao Binaria");		// VAMOS FAZER ESTE O RESTO � PARA APAGAR
            puts("5 - Sair\n");
            do {
                printf("Opcao: ");
                scanf(" %d", &opcao2);
                printf("%d\n", opcao2);
            } while (opcao2 < 1 || opcao2 > 5);

            // Faz um ciclo com o n�mero de execu��es definidas
            for (r=0; r<runs; r++) {
                // Gera��o da popula��o inicial
                pop = init_pop(param);
                // Avalia a popula��o inicial (qualidade = media distancia)
                evaluate(pop, param, mat);
                // Gera��o actual � a primeira (Usado ciclo while de otimizacao)
                gen_actual=1;
                // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra)
                // para poder comparar com as restantes
                best_run = pop[0];
                // Encontra-se a melhor solu��o dentro de toda a popula��o
                best_run = get_best(pop, param, best_run);
                // Reserva espa�o para os pais da popula��o seguinte
                parents = malloc(param.popsize * sizeof(chrom));
                // Se n�o conseguir alocar mem�ria, envia mensagem e fecha programa
                if (parents == NULL) {
                    printf("Erro na alocacao de memoria\n");
                    exit(1);
                }

                // Ciclo de optimiza��o. Corre at� n�mero de gera��es ser atingido
                while (gen_actual <= param.numGenerations) {
                    //printf("\nGeracao %d\n", gen_actual);
                    // Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
                    // Realiza torneios entre dois elementos escolhidos aleat�riamente e escolhe o melhor entre eles
                    // popsize vezes
                    tournament(pop, param, parents);
                    //tournament_tsize(pop, param, parents);
                    // Aplica os operados gen�ticos aos pais (os descendentes ficam armazenados  directamente na estrutura pop)
                    genetic_operators(parents, param, pop, opcao2);
                    // Avalia a nova popula��o (a dos filhos)
                    //printf("Nova avaliacao:\n");
                    evaluate(pop, param, mat);
                    // Actualiza a melhor solu��o encontrada
                    best_run = get_best(pop, param, best_run);
                    // Incrementa Gera��o Actual
                    gen_actual++;
                }
                // Contagem das solu��es inv�lidas - N�o aplic�vel
                //for (inv=0, i=0; i<param.popsize; i++)
                //    if (pop[i].valido == 0)
                //        inv++;

                // Escreve resultados da repeti��o que terminou
                printf("\nRepeticao %d", r+1);
                // Mostra melhor solu��o da repeti��o que terminou
                write_best(best_run, param);
                //printf("\nPercentagem Invalidos: %f\n", 100*(float)inv/EA_param.popsize); - N�o aplic�vel
                // Actualiza MBF somando o fitness da melhor solu��o
                mbf += best_run.fitness;
                // Atualiza a melhor solu��o global
                if (r==0 || best_run.fitness > best_ever.fitness)
                    best_ever = best_run;
                // Liberta mem�ria usada
                free(parents);
                free(pop);
            }
            // Beep
            MessageBeep(MB_OK);
        
            // Escreve resultados globais
            printf("\nMBF: %.3f\n", mbf/r);
            printf("\nMelhor solucao encontrada:\a\t");
            write_best(best_ever, param);

            break;
        }
        case 3:
        {
            // Metodo Hibrido
            printf("\n\nALGORITMO NAO IMPLEMENTADO");

            struct info param;
            pchrom      pop = NULL, parents = NULL;
            chrom       best_run, best_ever;
            int         gen_actual;
            float       mbf = 0.0;

            // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input - n�o aplicavel
            param.popsize = 100;              // Tamanho da popula��o
            param.pr = 1.0;                 // Probabilidade de recombina��o
            param.pm = 0.01;                 // Probabilidade de muta��o
            param.tsize = 2;                // Tamanho do torneio para sele��o do pai da pr�xima gera��o
            param.numGenerations = 2500;       // N�mero de gera��es
            param.numGenes = num_elem_conj; // N�mero de elementos (N�mero de objetos que se podem colocar na mochila)

            if (param.numGenes > MAX_OBJ) {
                printf("N�mero de elementos superior ao MAX_ELE\n");
                exit(1);
            }

            // Escolha algoritmo a utilizar
            printf("\n\n");
            puts("Introduza os algoritmos de recombinacao e mutacao a usar:");
            puts("1 - Metodo Evolutivo e Solucoes da Populacao Inicial Geradas pelo Trepa Colinas");
            puts("2 - Metodo Evolutivo e Solucoes Ultima Populacao Refinadas pelo Trepa Colinas");
            puts("3 - Trepa Colinas Aplicado a Algumas Solucoes Geradas pelo Metodo Evolutivo");
            puts("4 - Sair\n");
            do {
                printf("Opcao: ");
                scanf(" %d", &opcao2);
                printf("%d\n", opcao2);
            } while (opcao2 < 1 || opcao2 > 4);

            // Faz um ciclo com o n�mero de execu��es definidas
            for (r=0; r<runs; r++) {
                // Gera��o da popula��o inicial
                if (opcao2 == 1)
                    // pop = trepa-colinas
                    pop = init_pop(param); // Remover
                else
                    pop = init_pop(param);
                // Avalia a popula��o inicial (qualidade = media distancia)
                evaluate(pop, param, mat);
                // Gera��o actual � a primeira (Usado ciclo while de otimizacao)
                gen_actual=1;
                // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra)
                // para poder comparar com as restantes
                best_run = pop[0];
                // Encontra-se a melhor solu��o dentro de toda a popula��o
                best_run = get_best(pop, param, best_run);
                // Reserva espa�o para os pais da popula��o seguinte
                parents = malloc(param.popsize * sizeof(chrom));
                // Se n�o conseguir alocar mem�ria, envia mensagem e fecha programa
                if (parents == NULL) {
                    printf("Erro na alocacao de memoria\n");
                    exit(1);
                }

                // Ciclo de optimiza��o. Corre at� n�mero de gera��es ser atingido
                while (gen_actual <= param.numGenerations) {
                    // Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
                    // Realiza torneios entre dois elementos escolhidos aleat�riamente e escolhe o melhor entre eles
                    // popsize vezes
                    tournament(pop, param, parents);
                    //tournament_tsize(pop, param, parents);
                    // Aplica os operados gen�ticos aos pais (os descendentes ficam armazenados  directamente na estrutura pop)
                    genetic_operators(parents, param, pop, opcao2);
                    // Avalia a nova popula��o (a dos filhos)
                    //printf("Nova avaliacao:\n");
                    evaluate(pop, param, mat);
                    // Actualiza a melhor solu��o encontrada
                    best_run = get_best(pop, param, best_run);
                    // Incrementa Gera��o Actual
                    gen_actual++;
                }

                // Escreve resultados da repeti��o que terminou
                printf("\nRepeticao %d", r+1);
                // Mostra melhor solu��o da repeti��o que terminou
                write_best(best_run, param);
                //printf("\nPercentagem Invalidos: %f\n", 100*(float)inv/EA_param.popsize); - N�o aplic�vel
                // Actualiza MBF somando o fitness da melhor solu��o
                mbf += best_run.fitness;
                // Atualiza a melhor solu��o global
                if (r==0 || best_run.fitness > best_ever.fitness)
                    best_ever = best_run;
                // Liberta mem�ria usada
                free(parents);
                free(pop);
            }
           
            // Beep
            MessageBeep(MB_OK);
           
            // Escreve resultados globais
            printf("\nMBF: %.3f\n", mbf/r);
            printf("\nMelhor solucao encontrada:");
            write_best(best_ever, param);


            break;
        }
        case 4:
        {
            return 0;
            //break;
        }
    }
}
