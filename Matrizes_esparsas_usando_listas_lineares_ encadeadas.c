#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

// Trabalho sobre Matrizes Esparsas usando listas lineares encadeadas
// 
// Autores:
// Erick Lopes
// Jonathan Soares
// 
// Metodologia utilizada: Go Horse Process (GHP)


typedef struct nodo
{
    float dado;
    int lin, col;
    struct nodo *prox;
}Nodo;

/*Prototipos das funcoes*/
Nodo * Cria_Nodo();
void insert_node(Nodo **inicial_node, int lin, int col, float dado);
float get_node_value(Nodo **inicial_node, int lin, int col);
void imprime_lista(Nodo **inicial_node);
void clear_list(Nodo **inicial_node);
Nodo * op_matrix(Nodo **inicial_node1, Nodo **inicial_node2, float mult);
Nodo * mult_matrix(Nodo **inicial_node1, Nodo **inicial_node2, int tot_lin_1, int tot_col_1, int tot_col_2);
void gerar_matriz_transposta(Nodo **inicial_node, Nodo **resultado);
void imprime_matriz(Nodo **inicial_node, int num_linhas_matriz, int num_colunas_matriz);
void imprime_diagonal_principal(Nodo **inicial_node, int num_linhas_matriz, int num_colunas_matriz);
void print_exit_menu();
void clear_screen();

// Uma função que faz a alocação de memória para cada nodo criado na lista;
Nodo * Cria_Nodo(){
    Nodo *p;
    p = (Nodo *) malloc(sizeof(Nodo));
    if(!p)
    {
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

// Uma função que insere na lista um nodo alocado;
void insert_node(Nodo **inicial_node, int lin, int col, float dado){
	Nodo *new = NULL, *aux1, *aux2 = NULL; //aux1 para o nodo sendo verificado no momento
	// e aux2 para o anterior

	new = Cria_Nodo();
	new->lin = lin;
	new->col = col;
	new->dado = dado;
	new->prox = NULL;

	if (*inicial_node == NULL){ // primeiro e último
		*inicial_node = new;

	}else{
		aux1 = *inicial_node;
		while (1){
			if (aux1->lin > lin || (aux1->lin == lin && aux1->col > col)){
				if (dado == 0.0){
					free(new);
					return;
				}

				new->prox = aux1;

				if (aux2 != NULL){ // meio da matriz
					aux2->prox = new;
				} else { // novo primeiro da matriz
					*inicial_node = new;
				}

				break;
			}else if (aux1->lin == lin && aux1->col == col){
				if (dado == 0.0){
					if (aux2 != NULL){
						aux2->prox = aux1->prox;
					}else{
						*inicial_node = aux1->prox;
					}

					free(aux1);
					free(new);
				}else{
					new->prox = aux1->prox;
					if (aux2 != NULL){
						aux2->prox = new;
					}

					free(aux1);
				}
				break;
			} else if (aux1->prox == NULL){ // novo último
				if (dado == 0.0){
					free(new);
					return;
				}

				aux1->prox = new;

				break;
			}
			aux2 = aux1; //guarda anterior
			aux1 = aux1->prox;
		}
	}
}

// função para retornar um valor da matriz. retorna 0 caso não exista na lista
float get_node_value(Nodo **inicial_node, int lin, int col){
	Nodo *aux;
	float dado = 0.0;

	if (*inicial_node != NULL){
		aux = *inicial_node;

		do{
			if (aux->lin == lin && aux->col == col){
				dado = aux->dado;
				break;
			}

			aux = aux->prox;
		} while (aux != NULL && (aux->lin < lin || (aux->lin == lin && aux->col <= col)));
	}

	return dado;
}

// Done: Uma função que lê (busca) os dados de uma lista; (EL)
void imprime_lista(Nodo **inicial_node)
{
    Nodo *nodo;

    nodo = *inicial_node;
    while (nodo != NULL)
    {
        printf("%s: Linha:%i Coluna:%i Dado:%.2f \n",__func__ , nodo->lin, nodo->col, nodo->dado);
        nodo = nodo->prox;
    }

    print_exit_menu();
}

// Done: Uma função que libera a memória alocada para a lista; (JS)
void clear_list(Nodo **inicial_node){
    Nodo *aux1;

    while (*inicial_node != NULL)
    {     
       aux1 = *inicial_node;
       *inicial_node = (*inicial_node)->prox;
       free(aux1);
    }
}

// Done: Uma função que lê os dados da matriz e insere na lista somente os dados diferentes de zero; (EL)
// Essa função está feita na main.




// Done: Uma função que soma duas matrizes; (JS)
// Done: Uma função que subtrai duas matrizes; (JS)
// soma/subtrai conforme parametro mult
Nodo * op_matrix(Nodo **inicial_node1, Nodo **inicial_node2, float mult){
	Nodo *list1_node, *list2_node, *new_list = NULL;
	int lin1, lin2, col1, col2;
	float resultado;

	if (*inicial_node1 != NULL && *inicial_node2 != NULL){
		list1_node = *inicial_node1;
		list2_node = *inicial_node2;

		while (list1_node != NULL && list2_node != NULL){
			lin1 = list1_node->lin;
			col1 = list1_node->col;
			lin2 = list2_node->lin;
			col2 = list2_node->col;

			if (lin1 == lin2 && col1 == col2){ // lin e col iguais; soma
				resultado = list1_node->dado + (list2_node->dado * mult);

				insert_node(&new_list, lin1, col1, resultado);

				list1_node = list1_node->prox;
				list2_node = list2_node->prox;
			}
			else if (lin1 > lin2 || (lin1 == lin2 && col1 > col2)){ // matrix 1 ahead
				insert_node(&new_list, lin2, col2, list2_node->dado * mult);

				list2_node = list2_node->prox;
			}
			else{ // matrix 2 ahead
				insert_node(&new_list, lin1, col1, list1_node->dado);

				list1_node = list1_node->prox;
			}
		}
	}

	return new_list;
}

// Done: Uma função que multiplica duas matrizes; (JS)
Nodo * mult_matrix(Nodo **inicial_node1, Nodo **inicial_node2, int tot_lin_1, int tot_col_1, int tot_col_2){
	Nodo *new_list = NULL;
	int counter_lin, counter_col, counter_mult;
	float valor1, valor2, total;

	for (counter_lin = 0; counter_lin < tot_lin_1; counter_lin++){
		for (counter_col = 0; counter_col < tot_col_2; counter_col++){
			total = 0;
			for (counter_mult = 0; counter_mult < tot_col_1; counter_mult++){
				valor1 = get_node_value(inicial_node1, counter_lin, counter_mult);
				valor2 = get_node_value(inicial_node2, counter_mult, counter_col);
				total += valor1 * valor2;
			}

			insert_node(&new_list, counter_lin, counter_col, total);
		}
	}

	return new_list;
}

// Done: Uma função que gera a matriz transposta; (EL)
void gerar_matriz_transposta(Nodo **inicial_node, Nodo **resultado)
{
    int linha_transposta, coluna_transposta;
	float dado;
    Nodo *aux = NULL;

    aux = *inicial_node;

    while(aux != NULL)
    {
        linha_transposta  = aux->col;
        coluna_transposta = aux->lin;
        dado              = aux->dado;

        insert_node(&(*resultado), linha_transposta, coluna_transposta, dado);

        aux = aux->prox;
    }
}

// Done: Uma função que imprime todos os dados da matriz, inclusive os zeros; (EL)
void imprime_matriz(Nodo **inicial_node, int num_linhas_matriz, int num_colunas_matriz)
{
    int i, j;
    Nodo *aux;
    bool found_match = false;

    printf("%s: Imprimindo Matriz completa! \n",__func__);

    for (i=0; i<num_linhas_matriz; i++)
    {
        for(j=0; j<=num_colunas_matriz; j++)
        {
            if( j < num_colunas_matriz )
            {
                aux = *inicial_node;
                found_match = false;

                while(aux != NULL)
                {
                    if(aux->lin == i && aux->col == j)
                    {
                        found_match = true;
                        printf(" %.1f     ", aux->dado);
                        break;
                    }
                    else
                        aux = aux->prox;
                }

                if(!found_match)
                {
                    printf(" 0.0     ");
                }
            }
            if( j == num_colunas_matriz )
            {
                printf("\n");
            }
        }
    }

    print_exit_menu();
}
// ToDo: Uma função que imprime os elementos da diagonal principal, inclusive os zeros caso existam (EL)
void imprime_diagonal_principal(Nodo **inicial_node, int num_linhas_matriz, int num_colunas_matriz)
{
    int i, j;
    Nodo *aux;
    bool found_match = false;

    printf("%s: Imprimindo diagonal principal! \n",__func__);

    for (i=0; i<num_linhas_matriz; i++)
    {
        for(j=0; j<=num_colunas_matriz; j++)
        {
            if( j < num_colunas_matriz )
            {
                if( i == j )
                {
                    aux = *inicial_node;
                    found_match = false;

                    while(aux != NULL)
                    {
                        if(aux->lin == i && aux->col == j)
                        {
                            found_match = true;
                            printf(" %.1f     ", aux->dado);
                            break;
                        }
                        else
                            aux = aux->prox;
                    }

                    if(!found_match)
                    {
                        printf(" 0.0     ");
                    }
                }
                else
                {
                    printf("         ");
                }
            }
            if( j == num_colunas_matriz )
            {
                printf("\n");
            }
        }
    }

    print_exit_menu();
}

void print_exit_menu()
{
    int option;
    while(1)
    {
        printf("\n0. Sair\n");
        scanf("%i", &option);
        if(option == 0)
            break;
    }
}

void clear_screen(){
    #ifdef WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main(){
    Nodo *lista1 = NULL, *lista2 = NULL, *auxNode = NULL;
    int option, lin, col, num_lista, exit_menu;
    int num_linhas_lista_1, num_linhas_lista_2, num_colunas_lista_1, num_colunas_lista_2;
    float valor;

    do{
        clear_screen();

        printf("\n1. Inserir valor na lista");
        printf("\n2. Somar matrizes");
        printf("\n3. Subtrair matrizes");
        printf("\n4. Multiplicar matrizes");
        printf("\n5. Imprimir matriz completa");
        printf("\n6. Imprimir lista");
        printf("\n7. Imprimir Diagonal Principal");
        printf("\n8. Gerar a matriz transposta");
        printf("\n0. Sair\n");

        
        clear_list(&auxNode);

        scanf("%i", &option);
        switch (option){
            case 0:
                printf("\nFechando programa...\n");
                /*desalocar memoria aqui*/
				clear_list(&lista1);
				clear_list(&lista2);
                clear_list(&auxNode);
                return 1;
                break;
            /*Inserir Valor na Lista*/
            case 1:
                clear_screen();

                printf("\nEm qual lista deseja inserir? 1 ou 2?");
                scanf("%i", &num_lista);

                do{
                    clear_screen();

                    printf("\nLinha?");
                    scanf("%i", &lin);
                    printf("\nColuna?");
                    scanf("%i", &col);

                    printf("\nValor?");
                    scanf("%f", &valor);

                    if(num_lista == 1){
                        insert_node(&lista1, lin, col, valor);
                    }
                    else if(num_lista == 2){
                        insert_node(&lista2, lin, col, valor);
                    }

                    clear_screen();
                    printf("\n0. Sair ");
                    printf("\n1. Adicionar mais elementos na lista %i \n", num_lista);
                    scanf("%i", &exit_menu);

                } while(exit_menu);

                if(num_lista == 1){
                    clear_screen();
                    printf("\nQual o número total de linhas da matriz 1?");
                    scanf("%i", &num_linhas_lista_1);
                    printf("\nQual o número total de colunas da matriz 1?");
                    scanf("%i", &num_colunas_lista_1);
                }
                else if(num_lista == 2){
                    clear_screen();
                    printf("\nQual o número total de linhas da matriz 2?");
                    scanf("%i", &num_linhas_lista_2);
                    printf("\nQual o número total de colunas da matriz 2?");
                    scanf("%i", &num_colunas_lista_2);
                }

                break;
            /*Somar Matrizes*/
            case 2:
				if (num_linhas_lista_1 == num_linhas_lista_2 && num_colunas_lista_1 == num_colunas_lista_2){
					auxNode = op_matrix(&lista1, &lista2, 1.0);

					imprime_matriz(&auxNode, num_linhas_lista_1, num_colunas_lista_1);
				}
				else{
					clear_screen();
					printf("\nNão é possível somar matrizes com número de linhas ou colunas diferentes.\n");
					print_exit_menu();
				}
				break;
            /*Subtrair Matrizes*/
            case 3:
				if (num_linhas_lista_1 == num_linhas_lista_2 && num_colunas_lista_1 == num_colunas_lista_2){
					auxNode = op_matrix(&lista1, &lista2, -1.0);

					imprime_matriz(&auxNode, num_linhas_lista_1, num_colunas_lista_1);
				}
				else{
					clear_screen();
					printf("\nNão é possível subtrair matrizes com número de linhas ou colunas diferentes.\n");
					print_exit_menu();
				}
				break;
            /*Multiplicar Matrizes*/
            case 4:
				if (num_colunas_lista_1 == num_linhas_lista_2){
					auxNode = mult_matrix(&lista1, &lista2, num_linhas_lista_1, num_colunas_lista_1, num_colunas_lista_2);
					
					imprime_matriz(&auxNode, num_linhas_lista_1, num_colunas_lista_2);
				}
				else{
					clear_screen();
					printf("\nNão é possível multiplicar matrizes com número de linhas diferentes do número colunas.\n");
					print_exit_menu();
				}
				break;
            /*Imprimir Matriz completa*/
            case 5:
                clear_screen();

                printf("\nQual matriz completa deseja imprimir? 1 ou 2?");
                scanf("%i", &num_lista);

                clear_screen();
                if(num_lista == 1){
                    imprime_matriz(&lista1, num_linhas_lista_1, num_colunas_lista_1);
                }
                if(num_lista == 2){
                    imprime_matriz(&lista2, num_linhas_lista_2, num_colunas_lista_2);
                }

                break;
            /*Imprimir lista*/
            case 6:
                clear_screen();

                printf("\nQual lista deseja imprimir? 1 ou 2?");
                scanf("%i", &num_lista);

                if(num_lista == 1){
                    imprime_lista(&lista1);
                }
                if(num_lista == 2){
                    imprime_lista(&lista2);
                }
                break;
            /*Imprimir diagonal principal*/
            case 7:
                clear_screen();

                printf("\nA diagonal principal de qual matriz deseja imprimir? 1 ou 2?");
                scanf("%i", &num_lista);

                clear_screen();
                if(num_lista == 1){
                    if(num_linhas_lista_1 == num_colunas_lista_1){
                        imprime_diagonal_principal(&lista1, num_linhas_lista_1, num_colunas_lista_1);
                    }
                    else{
                        clear_screen();
                        printf("\nNão é possível mostrar a diagonal principal de uma matriz não quadrada.\n");
                        print_exit_menu();
                    }
                }
                if(num_lista == 2){
                    if(num_linhas_lista_2 == num_colunas_lista_2){
                        imprime_diagonal_principal(&lista2, num_linhas_lista_2, num_colunas_lista_2);
                    }
                    else{
                        clear_screen();
                        printf("\nNão é possível mostrar a diagonal principal de uma matriz não quadrada.\n");
                        print_exit_menu();
                    }
                }

                break;
            /*Gerar matriz transposta*/
            case 8:
                clear_screen();

                printf("\nQual lista deseja gerar a matriz transposta? 1 ou 2?");
                scanf("%i", &num_lista);

                if(num_lista == 1){
                    gerar_matriz_transposta(&lista1, &auxNode);
                    imprime_matriz(&auxNode, num_colunas_lista_1 ,num_linhas_lista_1);
                }
                if(num_lista == 2){
                    gerar_matriz_transposta(&lista2, &auxNode);
                    imprime_matriz(&auxNode, num_colunas_lista_2 ,num_linhas_lista_2);
                }

                break;

            default:
                printf("Opcao invalida.");
                break;
        }
    }while(option);

    return 0;
}
