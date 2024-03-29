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

#ifndef TOKEN
#define TOKEN

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using std::cout;
using std::cin;
using std::string;
using std::istringstream;
using std::ostringstream;

/////////////////////////////////////////////////////////////////////
// Tipi di dato semplici
/////////////////////////////////////////////////////////////////////

enum kind {PARENTESI_APERTA, PARENTESI_CHIUSA, NUMERO, OP_SOMMA,
OP_SOTTRAZIONE, OP_MOLTIPLICAZIONE, SCONOSCIUTO};

struct token {
  string val;
  kind k;
};

/////////////////////////////////////////////////////////////////////
// Funzioni ausiliarie su tipi di dato semplici
/////////////////////////////////////////////////////////////////////

// da stringa a numero
int str2int(const string &s);

// da numero a stringa
string int2str(int n);

// ritorna false se e solo se token di tipo sconosciuto o se stringa vuota.
// se stringa non vuota, la stringa perde il pezzo iniziale,
// che diventa token.

// ritorna false se e solo se non si può leggere un token dall'input stream;
// inserisce in t il token letto
bool prossimoToken(istringstream &iss, token &t);

#endif /* TOKEN */
