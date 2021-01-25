struct info init_dados(char *nome, int mat [][10]);
void init_rand();
//void mostra_matriz(int n, float** matriz);

// FUNCOES COMUNS ALGORITMOS
int random_l_h(int min, int max);
int flip();

// ALGORITMO TREPA-COLINAS
void gera_sol_inicial(int *sol, int v);
void escreve_sol(int *sol, int num_elem_conj);
void substitui(int a[], int b[], int n);

// ALGORTIMO EVOLUTIVO
pchrom init_pop(struct info d);
chrom get_best(pchrom pop, struct info d, chrom best);
float rand_01();
void write_best(chrom x, struct info d);
int existe_val_arr(int val, int* mat, int tam);

