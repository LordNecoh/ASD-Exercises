// trucchetto per fare contento l'editor, che altrimenti si lamenta che le strutture dati non sono definite
#ifndef USE_HASH_TABLE
#define USE_HASH_TABLE
#endif

#include "dictionary.h"

#include <math.h> // sqrt()

using namespace dict;

struct dict::cell {
      Elem	elem;
 	   cell	*left;
	   cell	*right;
};


/*              FUNZIONE NON IMPLEMENTATA                       */
/* Ritorna OK se la chiave non esisteva già e se l'inserimento  */
/* ha avuto successo.							    */
/* Ritorna FAIL se la chiave esisteva già o se l'inserimento    */
/* non ha avuto successo.*/


Error dict::insertElem(const Key k, const Value v,  Dictionary& s)
{
	if(s == emptyBtree){ 
		//Inserimento
		cell *aux = new cell;
		aux->elem.key = k;
		aux->elem.value = v;
		aux->left = nullptr;
		aux->right = nullptr;
		s=aux;
		return OK;
	}
	if(s->elem.key == k) return FAIL;
	if(k < s->elem.key) return insertElem(k, v, s->left);
	//(Altrimenti)
	return insertElem(k, v, s->right);
	
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/* Ritorna il Value v associato alla Key k, se questa esiste    */
/* Ritorna emptyValue altrimenti					    */
/****************************************************************/
Value dict::search(const Key k, const Dictionary& s)
{	
	if (s == nullptr) return emptyValue;
	if (s->elem.key==k) return s->elem.value;
	if (s->elem.key<k) return search(k, s->right);
	//(Altrimenti)
	return search(k, s->left);
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/* Ritorna OK se la chiave esiste nel dizionario e se la 	    */
/* cancellazione ha avuto successo					    */
/* Ritorna FAIL se la chiave non esisteva nel dizionaio o se la */
/* cancellazione non ha avuto successo				    */
/****************************************************************/

Elem deleteMin(Dictionary& s){
	if(s->left != nullptr){
		return deleteMin(s->left);
	}
	if(s->right != nullptr){
	    return deleteMin(s->right);
	}
	Btree aux = s;
	delete s;
	return aux->elem;
}

Error dict::deleteElem(const Key k, Dictionary& s)
{
  if (s==emptyBtree) return FAIL;
  if (s->elem.key==k){
	// Rimuovo sottoalbero
	if(s->right == nullptr && s->left == nullptr){ //Caso Foglia
		delete s;
		s = nullptr;
		return OK;
	}
	
	if(s->right != nullptr && s->left != nullptr){ //Caso due figli
		s-> elem = deleteMin(s->right);
		return OK;
	}
	
	//Caso un figlio
	if(s->right != nullptr){ //Figlio a destra
		Btree aux = s->right;
		delete s;
		s = aux;
		return OK;
	}
	
	//Figlio a sinistra
	Btree aux = s->left;
	delete s;
	s = aux;
	return OK;
  }  	
  if (s->elem.key>k) return deleteElem(k, s->left);
  //Altrimenti
  return deleteElem(k, s->right); 
}



/****************************************************************/
Dictionary dict::createEmptyDict()
{
   return emptyBtree;
}


/****************************************************************/
Dictionary readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs) {cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; return createEmptyDict();}  
    return readFromStream(ifs);
}


/****************************************************************/
Dictionary readFromStdin()
{
    cout << "\nInserire una sequenza di linee che rispettano la sintassi key: value.<enter>\nDigitare CTRL^ D per terminare l'inserimento\n";
    Dictionary d = readFromStream((std::cin));
// Questa serve per aggirare un bug di alcune versioni attuali di glibc.
    clearerr(stdin);
    return d;
}


/****************************************************************/
Dictionary readFromStream(istream& str)
{
    Dictionary d = createEmptyDict();     
    string key, kcopy;
    string value;
    getline (str, key, ':'); 
    getline (str, value); 
    while (!str.eof())  
        {     
        kcopy = key;
        removeBlanksAndLower(kcopy);   
        insertElem(kcopy, value, d); // FINCHE' NON IMPLEMENTATE LA INSERTELEM, NON PUO' FUNZIONARE CORRETTAMENTE: la insertElem e' la prima funzione che dovete implementare
        getline (str, key, ':'); 
        getline (str, value); 
        }
    str.clear();
    return d;
}


/****************************************************************/
int printBtree(Btree b) // stampa il contenuto di un Albero e restituisce il numero di elementi contenuti nel bucket stesso
{  
   return 0;
}


/****************************************************************/
void print(const Dictionary& s)
// stampa il contenuto del dizionario ed anche informazioni su come sono stati organizzati gli elementi
{
    if(s == nullptr) return;
    print(s->left);
    cout << s->elem.key << s->elem.value << endl;
    print(s->right);
}



