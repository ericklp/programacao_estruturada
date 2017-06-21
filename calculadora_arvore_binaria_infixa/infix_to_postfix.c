/*  Simple lib to perform conversion from infix to postfix notation                                                              */
/*  Shunting-yard algorithm copied from:                                                                                         */
/*  http://www.thecrazyprogrammer.com/2014/02/c-program-and-algorithm-for-conversion-of-an-expression-from-infix-to-postfix.html */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "infix_to_postfix.h"
 
void infix_to_postfix(char infix[],char postfix[])
{
    infix_stack s;
    char x,token;
    int i,j;    //i-index of infix,j-index of postfix
    infix_init(&s);
    j=0;
 
    for(i=0;infix[i]!='\0';i++)
    {
        token=infix[i];
        if(isalnum(token))
            postfix[j++]=token;
        else
            if(token=='(')
               infix_push(&s,'(');
        else
            if(token==')')
                while((x=infix_pop(&s))!='(')
                      postfix[j++]=x;
                else
                {
                    while(infix_precedence(token)<=infix_precedence(infix_top(&s))&&!infix_empty(&s))
                    {
                        x=infix_pop(&s);
                        postfix[j++]=x;
                    }
                    infix_push(&s,token);
                }
    }
 
    while(!infix_empty(&s))
    {
        x=infix_pop(&s);
        postfix[j++]=x;
    }
 
    postfix[j]='\0';
}
 
int infix_precedence(char x)
{
    if(x=='(')
        return(0);
    if(x=='+'||x=='-')
        return(1);
    if(x=='*'||x=='/'||x=='%')
        return(2);
 
    return(3);
}
 
void infix_init(infix_stack *s)
{
    s->top=-1;
}
 
int infix_empty(infix_stack *s)
{
    if(s->top==-1)
        return(1);
 
    return(0);
}

int infix_full(infix_stack *s)
{
    if(s->top==MAX-1)
        return(1);
 
    return(0);
}
 
void infix_push(infix_stack *s,int x)
{
    s->top=s->top+1;
    s->data[s->top]=x;
}
 
int infix_pop(infix_stack *s)
{
    int x;
    x=s->data[s->top];
    s->top=s->top-1;
    return(x);
}
 
int infix_top(infix_stack *p)
{
    return (p->data[p->top]);
}