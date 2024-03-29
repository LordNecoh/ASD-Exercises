#include "queue-array.h"

using namespace queue;


/****************************************************************/
/* 			FUNZIONI SULLE CODE 			*/
/****************************************************************/
//Funzioni Ausiliarie:
/****************************************************************/



void copyArray(const Queue& l, Queue& aux){
	for(int i = 0; i<l.size; ++i){
		aux.queue[i] = l.queue[i];
	}
}

/****************************************************************/
void shift(Queue& l){
	for( int i = l.size; i > 0; --i){
		l.queue[i] = l.queue[i-1];
	}
}

/****************************************************************/
void add(Queue& l, Elem e){
	shift(l);
	l.queue[0] = e;
	++l.size;
}

/****************************************************************/
void resize(Queue& l){
	Queue aux;
   	aux.size = l.size;
	aux.maxsize = (l.maxsize *2);
	aux.queue = new Elem[aux.maxsize];
	copyArray(l, aux);
	l = aux;
   	
}

/****************************************************************/
Queue queue::createEmpty()
{
   Queue q;
   q.size = 0;
   q.maxsize = 5;
   q.queue = new Elem[q.maxsize];
   return q;
}

/****************************************************************/
void queue::enqueue(Elem e, Queue& l) // aggiunge e in coda
{
	if( l.size < l.maxsize){
		add(l , e);
		return;
	}
	resize(l);
	add(l,e);
}


/****************************************************************/
Elem queue::dequeue(Queue& l) // rimuove il primo elemento e lo restituisce
{
	if(isEmpty(l)) return EMPTYELEM;
	--l.size;
	return l.queue[l.size];
	
}


/****************************************************************/
Elem queue::first(Queue& l) // restituisce il primo elemento
{
   	if(isEmpty(l)) return EMPTYELEM;
	return l.queue[l.size-1];
}


/****************************************************************/
bool queue::isEmpty(const Queue& l)
{
	if(l.size == 0) return true;
	return false;
}



/****************************************************************/
Queue readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // stream associato a un file, in lettura
    return readFromStream(ifs);
}



Queue readFromStdin()
{
    cout << endl << "inserire una sequenza di numeri separati da spazi seguiti da "
      <<  FINEINPUT << " per terminare" << endl;
    return readFromStream((std::cin));
}



Queue readFromStream(istream& is)
{
   Queue l = createEmpty();        
   Elem e;
   while (is >> e) {
      if (e == FINEINPUT) // il segnale di fine input è FINEINPUT
         return l;
      enqueue(e, l);
   }
   throw runtime_error("errore inserimento dati");
}



void print(const Queue& l)
{
   for (int i=0; i<l.size; ++i)
      cout << l.queue[i] << "; "; 
   cout << endl;
}
