/////////////////////////////////////////////////////////////////////
//
// Laboratorio 4: calcolo di semplici espressioni numeriche
//                mediante pile e code
//
// vengono accettati dall'esterno solo numeri positivi.
// Per trattare anche numeri negativi occorre modificare
// l'analizzatore lessicale.
//
/////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "queue.h"
#include "stack.h"
#include "token.h"

using std::cin;
using std::cout;
using std::istringstream;
using std::ostringstream;
using std::string;

/////////////////////////////////////////////////////////////////////
// Funzioni principali: leggi() e calcola()
/////////////////////////////////////////////////////////////////////

// Estrae uno dopo l'altro i token dalla stringa "str", inserendoli via via
// nella coda "codaToken"
bool leggi(const string &str, queue::Queue &codaToken) {
  token tempToken;
  istringstream iss(str);
  bool continua = true;

  while (continua) {
    continua = prossimoToken(iss, tempToken);
    if (!continua) return true;
    if (tempToken.k == SCONOSCIUTO) return false;
    queue::enqueue(tempToken, codaToken);
  }
  return true;
}

// Estrae uno dopo l'altro i token dalla coda, inserendoli via via sullo stack.
// OK Appena si incontra un token PARENTESI CHIUSA, quello segnala la fine OK di
// una sottoespressione; allora tiriamo giu' dallo stack gli ultimi cinque OK
// token inseriti. I token estratti dovrebbero essere esattamente, nell'ordine:
// OK un ")", un numero, un operatore aritmetico, un altro OK numero, e un "(";
// se non Ã¨ cosÃ¬, allora si segnala OK errore sintattico e l'algoritmo
// termina. OK A questo punto la funzione deve convertire i token di tipo NUMERO
// in numeri OK interi, eseguire l'operazione aritmetica opportuna, e
// trasformare il OK risultato da numero a token e inserire quest'ultimo sullo
// stack. Alla fine se OK non ci sono errori sintattici la funzione deve mettere
// nella variabile OK risultato il valore dell'espressione

bool calcola(queue::Queue codaToken, int &risultato) {
  stack::Stack pila;
  bool chiusa = false;  //Booleana di controllo per parentesi chiuse, metodo forse un po' barbaro, ma funzionale
  
  token tempToken = codaToken->prev->elem;
  if (tempToken.k == NUMERO && codaToken->next->next == codaToken) { //Nel caso ci sia un solo numero, esso è il risultato
    risultato = str2int(tempToken.val);
    return true;
  }

  while (!queue::isEmpty(codaToken)) {
    tempToken = queue::dequeue(codaToken);

    if (tempToken.k == PARENTESI_CHIUSA) { 
    // Estrazione quattro token per calcolo
      try {
        chiusa = true;
        int ris;
        token auxToken;
        
        // Estrazione Primo Operando:
        auxToken = pop(pila);
        if (auxToken.k != NUMERO) return false;
        int num1 = str2int(auxToken.val);

        // Estrazione Operatore:
        auxToken = pop(pila);
        if (auxToken.k == NUMERO || auxToken.k == PARENTESI_APERTA || auxToken.k == PARENTESI_CHIUSA) return false;
        kind op = auxToken.k;

        // Estrazione Secondo Operando:
        auxToken = pop(pila);
        if (auxToken.k != NUMERO) return false;
        int num2 = str2int(auxToken.val);

        // Estrazione Parentesi:
        auxToken = pop(pila);
        if (auxToken.k != PARENTESI_APERTA) return false;
        switch (op) {
        case OP_MOLTIPLICAZIONE:
          ris = num2 * num1;
          break;
          
        case OP_SOMMA:
          ris = num2 + num1;
          break;
          
        case OP_SOTTRAZIONE:
          ris = num2 - num1;
          break;
        }
        //Controllo che prima del token di risultato non ci sia roba insensata, utile nel caso non siano rimaste operazioni da svolgere
        if(stack::top(pila).k == PARENTESI_CHIUSA || stack::top(pila).k == NUMERO || (stack::top(pila).k == PARENTESI_APERTA && queue::isEmpty(codaToken))) return false;
        token risToken;
        risToken.val = int2str(ris);
        risToken.k = NUMERO;
        stack::push(risToken, pila);
      } catch (...) {
        return false;
      }
    }
    else {  //Se non c'è una parentesi chiusa ma c'è altro, impilo il token e segno che servirà un'altra chiusura
      stack::push(tempToken, pila);
      chiusa = false;
    }
  }
  if (!chiusa) return false;    //Se non ho mai ricevuto parentesi chiuse, vi è un errore (caso un solo numero già gestito prima)
  try {
    risultato = str2int(pop(pila).val);
  } catch (...) {
    return false;
  }
  if(!stack::isEmpty(pila)) return false;   //Se prima di terminare il calcolo sono presenti altri oggetti, vi è un errore
  return true;
}

/////////////////////////////////////////////////////////////////////
//                               MAIN
/////////////////////////////////////////////////////////////////////

int main() {
  string s;
  queue::Queue coda;
  int r;
  queue::createEmpty(coda);
  cout << "Inserisci l'espressione che vuoi valutare: " << endl;
  getline(cin, s);

  if (!leggi(s, coda)) {
    cerr << "errore lessicale" << endl;
    exit(1);
  }

  if (!calcola(coda, r)) {
    cerr << "errore sintattico" << endl;
    exit(2);
  }

  cout << "risultato: " << r << endl;
  exit(0);
}

