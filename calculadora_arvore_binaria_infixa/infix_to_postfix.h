/*  Simple lib to perform conversion from infix to postfix notation                                                              */
/*  Shunting-yard algorithm copied from:                                                                                         */
/*  http://www.thecrazyprogrammer.com/2014/02/c-program-and-algorithm-for-conversion-of-an-expression-from-infix-to-postfix.html */

#define MAX 50

typedef struct infix_stack
{
    int data[MAX];
    int top;
}infix_stack;

int infix_precedence(char);
void infix_init(infix_stack *);
int infix_empty(infix_stack *);
int infix_full(infix_stack *);
int infix_pop(infix_stack *);
void infix_push(infix_stack *,int);
int infix_top(infix_stack *);   //value of the top element
void infix_to_postfix(char infix[],char postfix[]);