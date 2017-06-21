/*

Alunos: 
Erick Lopes

Função que gera uma árvore binária a partir de uma expressão
aritmética em notação infixa.  Infixa (A+B) -> Posfixa(AB+)             OK

Função que avalia a árvore de expressão aritmética (calcula a
expressão).                                                             OK


o usuário deve entrar com quantas
expressões desejar, porém não deve ser informada a quantidade de expressões a
serem entradas no sistema. Após a entrada de todas as expressões que o usuário
desejar informar, o programa deverá apresentar a árvore gerada para cada
expressão e o resultado de cada expressão de entrada.                   OK

As expressões poderão conter operadores binário e unários e os operandos são
valores inteiros.                                                       NOK
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include "infix_to_postfix.h"

#define OPERAND 1
#define UN_OP   2
#define BIN_OP  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct tree
{
    char data;
    struct tree *left,*right;
};

typedef struct nodoFila {
    char exp[MAX];
    struct nodoFila *prox;
}Fila;

int top=-1;
struct tree *stack[MAX];
struct tree *node;

void push(struct tree* node)
{
    stack[++top]=node;
}

struct tree * pop()
{ 
    return(stack[top--]);
}

struct tree * Cria_Nodo_Tree() {
    struct tree *p;    
    p = (struct tree*) malloc(sizeof(struct tree));
    if (!p) {
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

Fila * Cria_Nodo_Fila() {
    Fila *p;
    p = (Fila *)malloc(sizeof(Fila));
    if (!p) {
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

void insere_no_fila(Fila **fila, char exp[MAX]) {
    Fila *novo = NULL;
    Fila *aux = NULL;

    novo = Cria_Nodo_Fila();
    strcpy(novo->exp, exp);
    novo->prox = NULL;

    if (*fila == NULL) { //primeiro e último
        *fila = novo;
    }
    else {
        aux = *fila;

        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }
}

void clear_list(Fila *fila) {
    Fila *aux1;

    while (fila != NULL) {
        aux1 = fila;
        fila = fila->prox;
        free(aux1);
    }
}

void clear_tree(struct tree *pai) {
    if (pai == NULL)
        return;

    if (pai->left != NULL)
        clear_tree(pai->left);
    if (pai->right != NULL)
        clear_tree(pai->right);
    free(pai);
}

/* Operadores unarios, binarios ou operandos */ 
int check(char ch)
{ 
    if (ch=='s'||ch=='l')
        return UN_OP;   
    if (ch=='+'||ch=='-'||ch=='/'||ch=='*')
        return BIN_OP;   
    else 
        return OPERAND;
}

/* Calculando o valor da expressão posfixa recursivamente */
int calculate(struct tree *node)
{
    int ch;
    ch=check(node->data);
    if (ch==OPERAND)
        return node->data-48;
    else if (ch==UN_OP)
    {
        if(node->data=='s')
            return (int)sqrt(calculate(node->right));
        if(node->data=='l')
            return (int)log10(calculate(node->right));
    }
    else if(ch==BIN_OP)
    {
        if(node->data=='+')
            return (calculate(node->left)) + (calculate(node->right));
        if(node->data=='-')
            return (calculate(node->left)) - (calculate(node->right));
        if(node->data=='*')
            return (calculate(node->left)) * (calculate(node->right));
        if(node->data=='/')
            return (calculate(node->left)) / (calculate(node->right));
    }

    return 0;
}

void inorder(struct tree *node)
{
    if (node!=NULL)
    {
        inorder(node->left);
        printf("%c",node->data);
        inorder(node->right);
    } 
}

/* Push operando*/
void operand(char ch)
{
    node        = Cria_Nodo_Tree();
    node->data  = ch;
    node->left  = NULL;
    node->right = NULL;
    push(node);
}

void operators(char ch)
{
    node        = Cria_Nodo_Tree();
    node->data  = ch;
    node->right = pop();
    if (check(node->data)!=UN_OP)
        node->left  = pop();
    push(node);
}

int main()
{
    int i,p,ans,numero_expressoes;
    Fila *expressoes = NULL;
    Fila *conversion = NULL;
    i = numero_expressoes = 0;

    char infix[MAX], postfix[MAX];
    
    
    printf("Digite as expressões na forma infixa: (0 para sair)\n");
    do {
        printf(":");
        scanf("%s", infix);

        if (infix[0] == '0')
            break;

        infix_to_postfix(infix,postfix);
        insere_no_fila(&expressoes, postfix);
    } while (1);


    while(expressoes!=NULL)
    {
        numero_expressoes++;
        strcpy(postfix,expressoes->exp);

        printf(" -------- Calculo Expressão %i --------", numero_expressoes);
        printf("\nExpressão pósfixa:            %s",postfix);

        for(i=0;postfix[i]!='\0';i++)
        {
            p=check(postfix[i]);
            if (p==OPERAND)
                operand(postfix[i]);
            else if (p==BIN_OP||p==UN_OP)
                operators(postfix[i]);
            else if (p==UN_OP)
                operators(postfix[i++]);
        }

        ans = calculate(stack[top]);
        printf("\nO resultado da expressão é:   " ANSI_COLOR_RED "%i" ANSI_COLOR_RESET,ans);
        printf("\nO percurso inorder é:         ");
        inorder(stack[top]);
        printf("\n\n");
        expressoes=expressoes->prox;
        
    }

    clear_list(expressoes);
    clear_tree(stack[top]);
}