/////////////////////////////////////////////////////////////////////
//
// Laboratorio 3: calcolo di semplici espressioni numeriche
//                mediante pile e code
//
// vengono accettati dall'esterno solo numeri positivi.
// Per trattare anche numeri negativi occorre modificare
// l'analizzatore lessicale.
//
///////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "token.h"

using std::cin;
using std::cout;
using std::istringstream;
using std::ostringstream;
using std::string;

/////////////////////////////////////////////////////////////////////
// Funzioni ausiliarie su tipi di dato semplici
/////////////////////////////////////////////////////////////////////

// da stringa a numero
int str2int(const string &s) {
  istringstream iss(s);
  int n;
  iss >> n;
  return n;
}

// da numero a stringa
string int2str(int n) {
  ostringstream otmp;
  otmp << n;
  return otmp.str();
}

// ritorna false se e solo se non si puÃ² leggere un token dall'input stream;
// inserisce in t il token letto
bool prossimoToken(istringstream &iss, token &t) {
  string temp;
  iss >> temp;
  if (!iss)
    return false;
  if (temp.size() > 1) {
    t.k = NUMERO;
    for (unsigned int i = 0; i < temp.size(); ++i) {
      if (!isdigit(temp[i])) {
        t.k = SCONOSCIUTO;
      }
    }
    t.val = temp;
    return true;
  }
  t.val = temp;
  switch (t.val[0]) {
  case '(':
    t.k = PARENTESI_APERTA;
    break;
  case ')':
    t.k = PARENTESI_CHIUSA;
    break;
  case '+':
    t.k = OP_SOMMA;
    break;
  case '-':
    t.k = OP_SOTTRAZIONE;
    break;
  case '*':
    t.k = OP_MOLTIPLICAZIONE;
    break;
  default:
    if (isdigit(t.val[0]))
      t.k = NUMERO;
    else {
      t.k = SCONOSCIUTO;
    }
    break;
  }
  return true;
}


