#include "stack.h"

using namespace stack;

struct stack::cell {
    token	elem;
    cell	*next;
};

/****************************************************************/
bool stack::isEmpty(const Stack& s)
{
    if(s == nullptr) return true;
    return false;
}

/****************************************************************/
void stack::push(const Elem x, Stack& s) 
{
    Stack aux = new stack::cell;
    aux->elem = x;
    aux->next = s;
    s = aux;
    return;
}

/****************************************************************/
Elem stack::pop(Stack& s)
{
    if(stack::isEmpty(s)) throw;
    Stack aux = s;
    s = s->next;
    Elem outElem = aux->elem;
    delete aux;
    return outElem;
}
Elem stack::top(Stack &s) {
    if (stack::isEmpty(s)){ //Se tento di vedere cosa c'è in cima alla lista vuota, genero un token che mi informa
    //Metodo non molto elegante, ma funzionante
        token emptyToken;
        emptyToken.val = "PILA VUOTA";
        emptyToken.k = SCONOSCIUTO;
        return emptyToken;
    }
    return s->elem;
}

/****************************************************************/
