#include "queue.h"

using namespace queue;

/****************************************************************/
/*                      FUNZIONI SULLE CODE                     */
/****************************************************************/

/****************************************************************/

void queue::createEmpty(Queue &l) {
  cell *aux = new cell;
  aux->next = aux;
  aux->prev = aux;
  l = aux;
}

void queue::enqueue(Elem e, Queue &l) // aggiunge e in coda
{
  cell *aux = new cell;
  aux->elem = e;
  aux->next = l->next;
  aux->prev = l;
  l->next->prev = aux; // NUOVO
  l->next = aux;
}

/****************************************************************/
Elem queue::dequeue(Queue &l) // rimuove il primo elemento e lo restituisce
{
  if (l->next == l)
    throw;
  Elem e;
  e = l->prev->elem;
  cell *aux = l->prev->prev;
  delete aux->next;
  aux->next = l;
  l->prev = aux;
  return e;
}

/****************************************************************/
bool queue::isEmpty(const Queue &l) {
  if (l->next == l)
    return true;
  return false;
}
/*
Elem queue::top(Queue &l){
  if(queue::isEmpty(l)) throw;
  return l->prev->elem;
}
*/
/****************************************************************/
