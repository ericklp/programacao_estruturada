// Listar o número de aviões esperando para decolar;                                            OK
// Listar o número de aviões esperando para pousar;                                             OK
// Autorizar a decolagem do primeiro avião na fila de decolagem;                                Ok
// Autorizar o pouso do primeiro avião na fila de pouso;                                        Ok
// Adicionar um avião na fila de espera para decolagem;                                         Ok
// Adicionar um avião na fila de espera para pouso;                                             Ok
// Listar todos os aviões que estão na fila de espera para decolagem, por ordem de chegada;     Ok
// Listar todos os aviões que estão na fila de espera para pouso, por ordem de chegada;         Ok
// Listar as características do primeiro avião da fila, antes de autorizar a sua decolagem;     Ok
// Listar as características do primeiro avião da fila, antes de autorizar o seu pouso;         Ok

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME 30

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define POUSAR "pous"
#define DECOLAR "decol"

typedef struct node{
	char nome[NAME];
	char identificacao[NAME/2];
	char origem[NAME*2];
	char destino[NAME*2];
	struct node *prox;
}Node;


Node * criaNode(){
	Node *p;
	p = (Node *) malloc(sizeof(Node));
	if(!p)
	{
		printf("Problema de alocação");
		exit(0);
	}
	return p;
}

void clear_list(Node **fila){
	Node *aux1;

	while (*fila != NULL)
	{
		aux1 = *fila;
		*fila = (*fila)->prox;
		free(aux1);
	}
}

/*Adicionar no final da fila - Enfileirar*/
void enqueue(Node **fila, char acao[6])
{
	Node *novo = NULL;
	Node *aux = NULL;

	char nome[NAME], identificacao[NAME / 2], origem[NAME * 2], destino[NAME * 2];

	printf("\nQual o NOME do avião? ");
	scanf("%s", nome);

	printf("\nQual a IDENTIFICAÇÃO do avião? ");
	scanf("%s", identificacao);

	printf("\nQual a ORIGEM do avião? ");
	scanf("%s", origem);

	printf("\nQual o DESTINO do avião? ");
	scanf("%s", destino);

	novo = criaNode();
	strcpy(novo->nome, nome);
	strcpy(novo->identificacao, identificacao);
	strcpy(novo->origem, origem);
	strcpy(novo->destino, destino);
	novo->prox          = NULL;

	if(*fila == NULL) //primeiro e último
		*fila = novo;
	else
	{
		aux = *fila;

		while(aux->prox != NULL){
			aux = aux->prox;
		}
		aux->prox = novo;
	}

	printf("\n Avião adicionado na fila de espera para %sagem!\n", acao);
}

/*Remover do começo da fila*/
void dequeue(Node **fila, char acao[6]){
	if(*fila != NULL)
	{
		*fila = (*fila)->prox;
		printf("!Avião %sou!\n", acao);
	}
	else
	{
		printf("\nTodos os aviões já %saram.\n", acao);
	}
}

int conta_elementos_filas(Node **fila)
{
	Node *node;
	node = *fila;
	int elem = 0;

	while (node != NULL){
		elem++;
		node = node->prox;
	}

	return elem;
}

void imprime_fila(Node **fila)
{
	Node *node;
	int elem = 0;

	node = *fila;
	while (node != NULL)
	{
		elem++;
		printf("Fila de Aviões: Posição [%i] Nome: [%s] Identificacao: [%s] Origem: [%s] Destino: [%s]\n",
			elem, node->nome, node->identificacao, node->origem, node->destino);

		node = node->prox;
	}
}

int imprime_primeiro_fila_e_autoriza(Node **fila, char acao[6])
{
	Node *node;
	node = *fila;
	int elem = 1;
	char resposta = 0;

	if(node != NULL)
	{
		//printf("Dados do avião que irá (decol/pous)ar: Posição ["ANSI_COLOR_GREEN"%i"ANSI_COLOR_RESET"] Nome: ["ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET"] Identificacao: ["ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET"] Origem: ["ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET"] Destino: ["ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET"]\n",
		printf("Dados do avião que irá %sar: Posição [%i] Nome: [%s] Identificacao: [%s] Origem: [%s] Destino: [%s]\n",
			acao, elem, node->nome, node->identificacao, node->origem, node->destino);
	}
	else
	{
		printf("\nTodos os aviões já %saram.\n", acao);
		return 0;
	}

	printf("\nAutorizar voô [s/N]? ");
	scanf(" %c", &resposta); // !!! do not remove this white space before %c !!!

	if(resposta == 's' || resposta == 'S' || resposta == 'Y' || resposta == 'y')
	{
		printf(ANSI_COLOR_GREEN "\nVoô autorizado!" ANSI_COLOR_RESET "\n" );
		return 1;
	}
	else
	{
		printf(ANSI_COLOR_RED "\nVoô não autorizado!" ANSI_COLOR_RESET "\n" );
		return 0;
	}

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
	Node *filaPouso = NULL, *filaDecolagem = NULL;
	int option;

	do{
		clear_screen();

		printf(  "1. Listar número de aviões esperando para decolar");
		printf("\n2. Listar número de aviões esperando para pousar\n");
		printf("\n3. Autorizar decolagem do primeiro avião");
		printf("\n4. Autorizar pouso do primeiro avião\n");
		printf("\n5. Adicionar avião na fila de espera de decolagem");
		printf("\n6. Adicionar avião na fila de espera de pouso\n");
		printf("\n7. Listar todos os aviões na fila para decolagem, em ordem de chegada");
		printf("\n8. Listar todos os aviões na fila para pouso, em ordem de chegada\n");
		printf("\n0. Fechar Programa\n");

		scanf("%i", &option);

		clear_screen();
		switch (option){
		case 0:
			printf("\nFechando programa...\n");
			/*desalocar memoria aqui*/
			clear_list(&filaPouso);
			clear_list(&filaDecolagem);
			return 1;

			/*Listar número de aviões esperando para decolar*/
		case 1:
			printf("O número de aviões esperando para decolar é de: %i aviões\n", conta_elementos_filas(&filaDecolagem));
			break;

			/*Listar número de aviões esperando para pousar*/
		case 2:
			printf("O número de aviões esperando para pousar é de: %i aviões\n", conta_elementos_filas(&filaPouso));
			break;

			/*Autorizar decolagem do primeiro avião*/
		case 3:
			if(imprime_primeiro_fila_e_autoriza(&filaDecolagem, DECOLAR))
				dequeue(&filaDecolagem, DECOLAR);
			break;

			/*Autorizar pouso do primeiro avião*/
		case 4:
			if(imprime_primeiro_fila_e_autoriza(&filaPouso, POUSAR))
				dequeue(&filaPouso, POUSAR);
			break;

			/*Adicionar avião na fila de espera de decolagem*/
		case 5:
			enqueue(&filaDecolagem, DECOLAR);

			break;

			/*Adicionar avião na fila de espera de pouso*/
		case 6:
			enqueue(&filaPouso, POUSAR);
			break;

			/*Listar todos os aviões na fila para decolagem, em ordem de chegada*/
		case 7:
			imprime_fila(&filaDecolagem);
			break;

			/*Listar todos os aviões na fila para pouso, em ordem de chegada*/
		case 8:
			imprime_fila(&filaPouso);
			break;

		default:
			printf("Opcao invalida.");
			break;
		}

		print_exit_menu();

	}while(option);

	return 0;
}