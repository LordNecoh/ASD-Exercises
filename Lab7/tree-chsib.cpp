#include "tree-chsib.h"

using namespace tree;

/* Nota: nei commenti usiamo "albero" e "puntatore a un nodo" come sinonimi, scegliendo di volta in volta
la formulazione più adatta a dare la specifica di una funzione. 
Dal punto di vista tecnico, usiamo sempre il tipo Tree sia quando operiamo su un albero, sia quando operiamo, 
a livello concettuale, su un puntatore a un nodo dell'albero. 
Questo approccio e' corretto in quanto un albero (o sottoalbero) è un puntatore a una struct di tipo treeNode:
un albero è identificato univocamente dal puntatore alla sua radice */ 



/*******************************************************************************************************/
// il nodo di un albero è caratterizzato dall'etichetta, il puntatore al primo figlio e il puntatore al prossimo fratello
// NON manteniamo il puntatore al padre, anche se sarebbe comodo per alcune funzioni (ma del tutto inutile per altre)
struct tree::treeNode {
    Label label;    
    treeNode *firstChild;
    treeNode *nextSibling;
};



/*******************************************************************************************************/
// isEmpty restituisce true se l'albero t e' vuoto, false altrimenti
bool tree::isEmpty(const Tree& t)
{ 
   return !t;
}

Tree tree::search(const Label l, const Tree& t) 
{ 
	  /* se l'albero e' vuoto non c'é niente da visitare */
	 if (isEmpty(t)) return emptyTree;
	 
	 if(t->label==l) return t;
	
	 Tree auxT = t->firstChild;
	    while (!isEmpty(auxT)) {
		Tree result = search(l, auxT);
		if (!isEmpty(result)) return result;
		auxT = auxT->nextSibling;
    }

	return emptyTree;
}
/*******************************************************************************************************/
// addElem aggiunge il nodo etichettato con labelOfNodeToAdd come
// figlio del nodo etichettato con labelOfNodeInTree.
// Caso particolare: aggiunta della radice, che si ottiene
// con labelOfNodeInTree uguale a emptyLabel (= nessun padre), e ha successo
// solo se l'albero e' vuoto (in caso contrario restituisce FAIL 
// e non aggiunge nulla).
// Restituisce FAIL se non esiste un nodo nell'albero etichettato 
// con labelOfNodeInTree oppure se nell'albero esiste gia' un nodo 
// etichettato con labelOfNodeToAdd; aggiunge e restituisce OK altrimenti
Error tree::addElem(const Label labelOfNodeInTree, const Label labelOfNodeToAdd, Tree& t)
{
	if(isEmpty(t)&&labelOfNodeInTree==emptyLabel){	//Aggiunta radice
		insert(labelOfNodeToAdd, t);
		return OK;
	}
	if(isEmpty(t)&&labelOfNodeInTree!=emptyLabel) return FAIL; //Aggiunta nodo con padre in albero vuoto
	if(!isEmpty(t)&&labelOfNodeInTree==emptyLabel) return FAIL; //Aggiunta di radice fallita
	if(member(labelOfNodeToAdd,t)) return FAIL;
	if(t->label == labelOfNodeInTree){      //Aggiunta nodo con padre
		if(t->firstChild == emptyTree){
			insert(labelOfNodeToAdd, t->firstChild);
			return OK;
		}
		Tree auxT = t->firstChild;
		while(auxT->nextSibling != emptyTree){
			auxT  = auxT->nextSibling;
		}
		insert(labelOfNodeToAdd, auxT->nextSibling);

		return OK;
	}
	//Scorrimento
	Tree auxT = t->firstChild;
	Error tempResult = FAIL;
	while (!isEmpty(auxT) && tempResult != OK) {
		tempResult = addElem(labelOfNodeInTree, labelOfNodeToAdd,auxT);
		auxT=auxT->nextSibling;
	}
	
	return tempResult;
}     


void tree::insert(const Label labelOfNodeToAdd, Tree& t){   //Per inserire effettivamente il nodo una volta arrivati nel punto giusto
	t=new treeNode;
	t->label=labelOfNodeToAdd;
	t->firstChild=emptyTree;
	t->nextSibling=emptyTree;
}


/*******************************************************************************************************/
// deleteElem (versione iterativa) rimuove dall'albero il nodo etichettato con la Label l
// e collega al padre di tale nodo tutti i suoi figli
// Restituisce FAIL se si tenta di cancellare la radice e questa ha 
// dei figli (non si saprebbe a che padre attaccarli) oppure se non esiste 
// un nodo nell'albero etichettato con la Label; cancella e restituisce OK altrimenti
Error tree::deleteElemI(const Label l, Tree& t)
{
     
	Tree tree2Delete=search(l,t);
	if(isEmpty(tree2Delete)) return FAIL;
	
	if(t->label==l){    //Cancellazione radice
		if(!isEmpty(t->firstChild))return FAIL;
		delete t;
		t=emptyTree;
		return OK;
	} 
	
	Tree auxT;
	Tree nuovaDest=search(father(l,t),t);
	list::list listaF=children(l,t);
	
	//metto i figli dell elemento da cancellare
	if(size(listaF)!=0){
		auxT=nuovaDest->firstChild;
		while(!isEmpty(auxT->nextSibling)) auxT=auxT->nextSibling;

		for(int i=0;i<size(listaF);i++){
			auxT->nextSibling=search(get(i,listaF),t);
			auxT=auxT->nextSibling;
		}
	}
	auxT=nuovaDest->firstChild;
	if(nuovaDest->firstChild->label==l)nuovaDest->firstChild=nuovaDest->firstChild->nextSibling;
	else{
			bool flag=true;
			while(auxT->nextSibling&& flag){
				if(auxT->nextSibling->label==l){
					auxT->nextSibling=auxT->nextSibling->nextSibling;
					flag=false;
				}else auxT=auxT->nextSibling;
			}
	}

	
	delete tree2Delete;
	tree2Delete=emptyTree;

     return OK;     
}



/*******************************************************************************************************/
// deleteElem (versione ricorsiva): stesso comportamento della versione iterativa, ma implementata ricorsivamente
// (può anche non essere ricorsiva la deleteElemR, ma deve fare uso di funzioni ausiliarie ricorsive)
//DA FARE
Error deleteElemRAux(Tree& tree2Delete, Tree& father,  Tree& auxT){

	if(auxT->nextSibling!=emptyTree) return deleteElemRAux(tree2Delete, father, auxT->nextSibling);
	
	//spostiamo i figli dell elemento da cancellare
	
	auxT->nextSibling=tree2Delete->firstChild;
	
	Tree auxT2=father->firstChild;
	if(auxT2==tree2Delete)father->firstChild=father->firstChild->nextSibling;
	else{
		while(auxT2->nextSibling!=tree2Delete){
			auxT2 = auxT2->nextSibling;
		}
		auxT2->nextSibling = tree2Delete->nextSibling;
	}
		
	delete tree2Delete;
	tree2Delete=emptyTree;
	return OK;
}

Error tree::deleteElemR(const Label l, Tree& t)
{	
	Tree tree2Delete=search(l,t);
	if(isEmpty(tree2Delete)) return FAIL;
	
	if(t->label==l){    //Cancellazione Radice
		if(!isEmpty(t->firstChild))return FAIL;
		delete t;
		t=emptyTree;
		return OK;
	} 
	
	
	Tree  padre = search(father(l,t),t);
	Tree auxT= padre->firstChild;
	return deleteElemRAux(tree2Delete, padre,auxT);
}




/*******************************************************************************************************/
// father restituisce l'etichetta del padre del nodo con etichetta l se il nodo esiste nell'albero 
// (o sottoalbero) t e se il padre esiste. 
// Restituisce la costante emptyLabel altrimenti
Label tree::father(const Label l, const Tree& t)
{ 
	
	if (isEmpty(t) || t->label == l) return emptyLabel;
    
	if(!isEmpty(t->firstChild)){
		if(t->firstChild->label==l) return t->label;
		
		Tree auxT = t->firstChild;
		Label a = father(l, auxT);
		if (a != emptyLabel) return a;

		while (auxT->nextSibling) {
			auxT = auxT->nextSibling;
			if (auxT->label==l)return t->label;
			a = father(l, auxT);
			if (a != emptyLabel) return a;

		}
	}
	
	return emptyLabel;
}



/*******************************************************************************************************/
// children restituisce una lista di Label, contenente le etichette 
// di tutti i figli nell'albero t del nodo etichettato con l
// La lista può essere implementata usando una qualunque delle strutture dati viste a lezione
// (non un Vector!!!): si riusi una implementazione del TDD list, adattandola 
// a elementi di tipo string (nelle precedenti esercitazioni gli elementi erano int)
list::List tree::children(const Label l, const Tree& t)
{
   list::List lst = list::createEmpty();
   Tree father=tree::search(l,t);
   
   if(isEmpty(father)||isEmpty(father->firstChild)) return lst;
   
   father=father->firstChild;
   while(father){
        addBack(father->label,lst);
        father=father->nextSibling;
   } 
   return lst;
}



/*******************************************************************************************************/
// degree restituisce il numero di archi uscenti dal nodo etichettato con l; restituisce -1 se non esiste 
// nessuna etichetta l nell'albero
int tree::degree(const Label l, const Tree& t)
{
   Tree father=tree::search(l,t);
   if(isEmpty(father)) return -1;
   
   int count=0;
   
   if(isEmpty(father->firstChild)) return count;
   
   father=father->firstChild;
   while(father){
   	count++;
   	father=father->nextSibling;
   } 
   return count;

}


/*******************************************************************************************************/
// ancestors (versione ricorsiva) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
// (può anche non essere ricorsiva la ancestorsR, ma deve fare uso di funzioni ausiliarie ricorsive)
void ancestorsRAux(const Label l, const Tree& t, list::list& lst)
{
   Label auxL=father(l,t);
   if(auxL !=emptyLabel){
   	addFront(auxL,lst);
   	ancestorsRAux(auxL,t,lst);
   }
}

list::List tree::ancestorsR(const Label l, const Tree& t)
{
   list::List lst = list::createEmpty();
   ancestorsRAux(l,t,lst);
   return lst;
}


/*******************************************************************************************************/
// ancestors (versione iterativa) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
list::List tree::ancestorsI(const Label l, const Tree& t)
{
   list::List lst = list::createEmpty();
    Label auxL=father(l,t);
   
   while(auxL!=emptyLabel){
   	addFront(auxL,lst);
   	auxL=father(auxL,t);
   }
   return lst;
}



/*******************************************************************************************************/
// leastCommonAncestor restituisce l'etichetta del minimo antenato comune 
// ai nodi etichettati con label1 e label2
Label tree::leastCommonAncestor(const Label label1, const Label label2, const Tree& t)
{
	list::List l1=ancestorsR(label1, t);
	int i=0;
	list::List l2=ancestorsR(label2, t);
	Label risultato=emptyLabel;
	while(i<size(l1)&&i<size(l2)){
		if(get(i,l1)!=get(i,l2)) return risultato;
		risultato =  get(i,l1);
		i++;

	}
		
  
	return risultato;
}



/*******************************************************************************************************/
// member restituisce true se il nodo etichettato con la label l appartiene all'albero t e false altrimenti
bool tree::member(const Label l, const Tree& t) 
{ 
	  /* se l'albero e' vuoto non c'é niente da visitare */
	 if (isEmpty(t)) return false;
	 
	 if(t->label==l) return true;
	
	 Tree auxT = t->firstChild;
	 while (!isEmpty(auxT)) {
	 	if(member(l,auxT)) return true;
	 auxT=auxT->nextSibling;
	 }
	 return false;
}



/*******************************************************************************************************/
// numNodes restituisce il numero di nodi nell'albero t mediante una visita ricorsiva in depthfirst
int tree::numNodes(const Tree& t) 
{
   /* se l'albero e' vuoto non c'é niente da visitare */
	 if (isEmpty(t)) return 0;
	
	 Tree auxT = t->firstChild;
	int count = 1;
	 while (!isEmpty(auxT)) {
	 count += numNodes(auxT);
	 auxT=auxT->nextSibling;
	 }
	return count; 
}



/*******************************************************************************************************/
// createEmptyTree restituisce l'albero vuoto
Tree tree::createEmpty()
{
   return emptyTree;
}



/*******************************************************************************************************/

void printTreeR(const Tree& t,int depth) {
	if (isEmpty(t)) return;
	
	for(int i=0; i<depth;i++){
		cout<<"--";
	}
	cout<<t->label<<endl;
	
	/* Chiamata ricorsiva di visit su ciascuno dei figli di t, partendo dal
	primo figlio e poi scorrendo la lista dei suoi fratelli */
	 Tree auxT = t->firstChild;
	 while (!isEmpty(auxT)) {
		printTreeR(auxT,depth+1);
		auxT=auxT->nextSibling;
	 }

}

// Visualizzazione possibilmente strutturata come visto a lezione
void printTree(const Tree& t) {
	printTreeR(t,0);

}




/****************************************************************/
// Questa funzione NON richiede di conoscere come e' fatta la struttura dati che implementa l'albero: usa solo addElem e quindi e' ad 
// un alto livello di astrazione
// Vantaggi: codice compatto, leggibile, riutilizzabile per qualunque implementazione del TDD Tree
// Svantaggi: inefficiente: la addElem cerca ogni volta il nodo con etichetta fatherLabel, mentre basterebbe una chiamata sola per 
// trovarlo e poi si potrebbero attaccare tutti i figli a esso, senza ripetere la dispendiosa ricerca nell'albero; per renderla 
// efficiente dovrei pero' accedere direttamente alla struttura dati chiamando qualche funzione ausiliaria che operi 
// sui nodi dell'albero, rinunciando a scrivere codice modulare, chiaro e ad un ragionevole livello di astrazione... 
// Non esiste un'opzione migliore tra le due: se il codice deve essere efficiente, è opportuno scrivere una funzione ad hoc per 
// una determinata implementazione; se il codice deve essere riutilizzabile, bisogna limitarsi a usare le funzioni offerte dall'
// interfaccia del TDD in modo che la funzione continui ad andare bene anche se cambia l'implementazione
Tree readFromStream(istream& str)
{
    Tree t = createEmpty(); 
    string line;
    Label rootLabel, fatherLabel, childLabel;
    getline(str, line);  
    istringstream instream;            // uso una variabile di tipo istringstream per poter scandire i pezzi di ogni riga usando >>
    instream.clear();
    instream.str(line);
    instream >> rootLabel;             // il primo elemento che si incontra nel file e' l'etichetta della radice, per convenzione su come deve essere fatto il file
    addElem(emptyLabel, rootLabel, t); // la si inserisce nell'albero vuoto, indicando che il padre non c'e' (primo argomento emptyLabel)
    getline(str, line);                // poi si iniziano a scansionare le righe seguenti
    instream.clear();
    instream.str(line);
    while (!str.eof())  
        {        
	instream >> fatherLabel; // in ogni riga del file, il primo elemento e' l'etichetta del nodo padre e gli altri sono le etichette dei figli
        normalize(fatherLabel); // normalizzo l'etichetta del padre
        while (!instream.eof())            // finche' la riga corrente non e' terminata
            { 
            instream >> childLabel;              // leggo la prossima etichetta
            normalize(childLabel);    // la normalizzo
            addElem(fatherLabel, childLabel, t); // e la attacco al padre
            }
        getline(str, line);  
        instream.clear();
        instream.str(line);
        }
    str.clear();
    return t;
}



/****************************************************************/
Tree readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs) {cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; return createEmpty();}  
    return readFromStream(ifs);
}



