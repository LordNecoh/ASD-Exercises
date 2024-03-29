// Implementa le funzioni sulle liste realizzate con strutture doppiamente collegate, circolari, con sentinella, senza ordinamento

// FILE DA MODIFICARE

#include "ASD-doubly-linked-list.h"


using namespace list;

struct list::node {  // descrivo qui, fuori dal namespace, come e' fatta la struttura node: i dettagli interni non saranno visibili dal main
    Elem info;
    node *prev;
    node *next;
};

/*************************************************/
/* Implementazione delle operazioni di utilita'  */
/*************************************************/

void readFromStream(istream& str, List& l) {
    createEmpty(l);
    Elem e;
    str>>e;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    // assumiamo che il segnale di fine input nel file sia  FINEINPUT
    while (e != FINEINPUT)  {     
        addRear(e, l);   // aggiunge l'elemento alla fine della lista
        str>>e;
        if (!str) throw runtime_error("Errore inserimento dati\n");
    }
}

/* legge il contenuto di una lista da file */
void readFromFile(string nome_file, List& l) {
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    readFromStream(ifs, l);
}


/* legge il contenuto di una lista da standard input */
void readFromStdin(List& l) {
   cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
   readFromStream((std::cin), l);
}

/* stampa la lista */
void print(const List& l) {
   List q = l->next;  // q "salta la sentinella" e va alla testa della lista
   while (q != l) {   // avanza finche' non finisce la lista
      cout << q->info << "; ";
      q = q->next; 
    }
}


/**************************************************/
/* Implementazione delle operazioni nel namespace */
/**************************************************/

/* crea la lista vuota */
void list::createEmpty(List& l) {
// Chiamabile solo se la lista e' effettivamente vuota
	List aux = new node;
	aux->next=aux;
	aux->prev=aux;
	l=aux;
}

/* restituisce true se la lista e' vuota (ed e' vuota se il next di l, e' l stessa */
bool list::isEmpty(const List& l) {
	return (l->next == l && l->prev == l);
}

 /* restituisce la dimensione della lista */
int list::size(const List& l) {
	if (l->next==l) return 0; 
	return 1+size(l->next);
}


/* "smantella" la lista svuotandola */
void list::clear(const List& l) {
	if (l->next==l && l->prev==l) return;
	clear(l->next);
	//Metto a posto i puntatori della cella prima
	List aux=l->prev;
	aux->next=l->next;
	//Metto a posto i puntatori della cella dopo
	List aux2=l->next;
	aux2->prev=l->prev;
	aux3 = l;
	delete aux3;	
}

/* restituisce l'elemento in posizione pos se presente; restituisce un elemento 
che per convenzione si decide che rappresenta l'elemento vuoto altrimenti*/
Elem list::get(int pos, const List& l) {
	List aux = l;
	for(int i=0; i<=pos && aux->next!=l; ++i){
	    if(i==pos) return aux -> info;
	    aux=aux->next;
	}
	return EMPTYELEM;
}

/* modifica l'elemento in posizione pos, se la posizione e' ammissibile */
void list::set(int pos, Elem e, const List& l) {
    List aux = l;
    int i=0;
    while(i<=pos && aux->next!=l){
        aux=aux->next;
        ++i;
    }
	if(i!=pos) return;
	aux->info = e;
}

/* inserisce l'elemento in posizione pos, shiftando a destra gli altri elementi */
void list::add(int pos, Elem e, const List& l) {                                               
    List aux = l;
    for(int i=0; i<pos && aux->next!=l; ++i) aux=aux->next;
    List aux2 = new node;
    aux2->info = e;
    aux2->prev = aux;
    aux2 -> next = aux->next;
    aux->next = aux2;
    aux2->next->prev=aux2;
}

/* inserisce l'elemento alla fine della lista */
void list::addRear(Elem e,  const List& l) {
    List aux = l;
    List newNode = new node;
    newNode->info = e;
    newNode->prev = aux->prev;
    newNode->next = aux;
    aux->prev = newNode;
    newNode -> prev -> next = newNode;
}

/* inserisce l'elemento all'inizio della lista */
void list::addFront(Elem e, const List& l) {
    List aux = l;
    List newNode = new node;
    newNode->info = e;
    newNode->prev = aux;
    newNode->next = aux->next;
    aux->next = newNode;
    newNode -> next -> prev = newNode;
}

/* cancella l'elemento in posizione pos dalla lista */
void list::removePos(int pos, const List& l) {
	List aux;
    for(int i=0; i<pos && aux->next!=l; ++i) aux=aux->next;
	aux->next=aux->next->next;
	aux->next->prev = aux;
	delete aux->next;	
}

 /* cancella tutte le occorrenze dell'elemento elem, se presenti, dalla lista */
void list::removeEl(Elem e, const List& l) {
    void list::removeEl(Elem e, const List& l) {
	List aux = l->next;
	if(aux ==l) return;
	if(aux->info == e){
		List temp=aux;
		temp->next->prev = temp->prev;
		temp->prev->next = temp->next;
		delete temp;
	}
	removeEl(e,aux);
}
}



