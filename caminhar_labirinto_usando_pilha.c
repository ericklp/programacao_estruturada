#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <time.h>

#define LIVRE 32  		//espaço
#define PAREDE 35		// #
#define VISITADA 46		// .
#define BECO 126		// ~
#define ATUAL 42		// *

#define NUM_LIN 30
#define NUM_COL 30

#define V_INI_SUM_X 0 // 0 -1 = baixo, direita, cima, esquerda 
#define V_INI_SUM_Y -1 // sempre será em sentido anti-horário

typedef struct node{
	int posXY;
	struct node *prox;
	int somaX;
	int somaY;
} *Node;

Node criaNode();
void push(Node * pilha, int posXY);
void pop(Node * pilha);
void clearPilha(Node * pilha);
void inicializa_matriz(int matriz[NUM_LIN][NUM_COL], int chanceLivre);
void mostra_matriz(int matriz[NUM_LIN][NUM_COL]);
int  procuraCaminho(Node * pilha, int matriz[NUM_LIN][NUM_COL]);
void clear_screen();
void delay();

Node criaNode(){
	Node p;
	p = (Node)malloc(sizeof(struct node));
	if (!p){
		printf("Problemas na Alocação!!!");
		exit(0);
	}
	return p;
}

void push(Node * pilha, int posXY){
	Node novo = criaNode();

	novo->posXY = posXY;
	novo->prox = *pilha;
	novo->somaX = V_INI_SUM_X;
	novo->somaY = V_INI_SUM_Y;
	*pilha = novo;
}

void pop(Node * pilha){
	Node aux;

	aux = *pilha;
	*pilha = (*pilha)->prox;
	free(aux);
	aux = NULL;
}

void clearPilha(Node * pilha){
	Node aux;
	while (*pilha != NULL){
		aux = *pilha;
		*pilha = (*pilha)->prox;
		free(aux);
		aux = NULL;
	}
}

void inicializa_matriz(int matriz[NUM_LIN][NUM_COL], int chanceLivre){
	int counter1, counter2;

	for (counter1 = 0; counter1 < NUM_LIN; counter1++)
		for (counter2 = 0; counter2 < NUM_COL; counter2++)
			matriz[counter1][counter2] = PAREDE;

	srand(time(NULL));

	for (counter1 = 1; counter1 < NUM_LIN - 1; counter1++)
		for (counter2 = 1; counter2 < NUM_COL - 1; counter2++)
			if (rand() % 100 < chanceLivre)
				matriz[counter1][counter2] = LIVRE;
			else
				matriz[counter1][counter2] = PAREDE;

	matriz[1][1] = LIVRE;
	matriz[NUM_LIN - 2][NUM_COL - 1] = LIVRE;
}

void mostra_matriz(int matriz[NUM_LIN][NUM_COL]){
	int counter1, counter2, counter3 = 0;
	char labirinto[NUM_COL + 1] = {'\0'};

	clear_screen();

	for (counter1 = 0; counter1 < NUM_LIN; counter1++){
		for (counter2 = 0; counter2 < NUM_COL; counter2++){
			labirinto[counter3] = matriz[counter1][counter2];
			counter3++;
		}
		printf("%s \n", labirinto);
		counter3 = 0;
	}

	printf("\n");
}

int procuraCaminho(Node * pilha, int matriz[NUM_LIN][NUM_COL]){
	int xToGo, yToGo, auxZ;

	do{
		matriz[(*pilha)->posXY / 100][(*pilha)->posXY % 100] = ATUAL; // First run: X=1 Y=1

		if ((*pilha)->posXY % 100 == NUM_COL - 1) // se localiza na saída
			return 1;

		auxZ = (*pilha)->somaX;
		(*pilha)->somaX = -(*pilha)->somaY;       // First run: somaX = 1
		(*pilha)->somaY = auxZ;                   // First run: somaY = 0.

		xToGo = (*pilha)->posXY / 100 + (*pilha)->somaX;  // First run: xToGo=2 (1+1)
		yToGo = (*pilha)->posXY % 100 + (*pilha)->somaY;  // First run: yToGo=1 (1+0)

		if (matriz[xToGo][yToGo] == LIVRE){
			mostra_matriz(matriz);
			matriz[(*pilha)->posXY / 100][(*pilha)->posXY % 100] = VISITADA;

			push(pilha, xToGo * 100 + yToGo);
            delay();
		}
		else if ((*pilha)->somaX == V_INI_SUM_X && // não existem mais
				(*pilha)->somaY == V_INI_SUM_Y){   // lados LIVRES
			mostra_matriz(matriz);
			matriz[(*pilha)->posXY / 100][(*pilha)->posXY % 100] = BECO;
			pop(pilha);
            delay();
		}

        delay();

	} while (*pilha != NULL); // enquanto existirem elementos na pilha

	return 0;
}

void clear_screen(){
	#ifdef WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void delay()
{
#ifdef WIN32
    Sleep(1);
#else
    usleep(30000);
#endif
}

int main(){
	int matriz[NUM_LIN][NUM_COL], chanceLivre, continuar = 0;
	Node pilha = NULL;

	do{
		clear_screen();
		printf("  0: Mais difícil possível\n");
		printf("100: Mais fácil possivel\n");
		printf("Escolha uma dificuldade de 0 a 100: ");
		scanf(" %i", &chanceLivre);

		do{
			inicializa_matriz(matriz, chanceLivre);
			mostra_matriz(matriz);

			printf("\n1. Continuar com esse labirinto.");
			printf("\n0. Criar outro labirinto.\n");
			scanf(" %i", &continuar);
		} while (!continuar);

		push(&pilha, 101);    // posição inicial

		if (procuraCaminho(&pilha, matriz)){
			mostra_matriz(matriz);
			printf("Saída encontrada!\n");
		}
		else{
			mostra_matriz(matriz);
			printf("Labirinto sem saída\n");
		}



		clearPilha(&pilha);

		printf("\n\n1. Executar novamente?");
		printf("\n0. Sair\n");
		scanf("%i", &continuar);
	} while (continuar);
}