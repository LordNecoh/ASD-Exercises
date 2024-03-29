#include <iostream>
#include <stdexcept>

#include "token.h"
	
using namespace std;

// Implementa QUEUE 
namespace queue {
    // Gli elementi della QUEUE sono token
    typedef token Elem;       
    struct cell{
	   token	elem;
	   cell	*next;
	   cell	*prev;
    };
    typedef cell* Queue; 
    //Elem top(Queue &l);
    void createEmpty(Queue& l);
    bool isEmpty(const Queue&);
    void enqueue(Elem, Queue&);  /* inserisce l'elemento alla fine della lista */
    Elem dequeue(Queue&);        /* cancella l'elemento in prima posizione (se esiste) e lo restituisce */
}
