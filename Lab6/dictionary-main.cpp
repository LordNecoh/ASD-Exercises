/////////////////////////////////////////////////////////////////////////////
//
// COME COMPILARE IL PROGRAMMA
//
// Per usare l'implementazione del dizionario basata su vettore ordinato:
//
// g++ -std=c++11 -Wall -DUSE_ORDERED_VECTOR dictionary-orderedvector.cpp string-utility.cpp dictionary-main.cpp
//
// Per usare l'implementazione del dizionario basata su lista doppiamente
// collegata con sentinella e ordinata:
//
// g++ -std=c++11 -Wall -DUSE_ORDERED_LIST dictionary-doublylinkedlist.cpp string-utility.cpp dictionary-main.cpp
//
// Per usare l'implementazione del dizionario basata su tabella hash:
//
// g++ -std=c++11 -Wall -DUSE_HASH_TABLE dictionary-hashtable.cpp string-utility.cpp dictionary-main.cpp
//
//
// ESECUZIONE
//
// eseguire il programma passando 'q' come primo parametro non stampa menu e prompt
//
/////////////////////////////////////////////////////////////////////////////



#include "dictionary.h"


const int maxreg = 10;
const string menu = "Avete a disposizione 10 registri numerati da 0 a 9 che contengono dei dizionari (chiamati anche array associativi o mappe), inizialmente vuoti.\nInserite l'operazione scelta e di seguito gli operandi richiesti.\na: leggi un dizionario da file [ogni linea del file deve avere la sintassi\n   `key: value` (2 operandi: nome_file indice);\nb: leggi un dizionario da standard input (1 operando: indice);\nc: inserisci una coppia (chiave, valore) in un dizionario (operazione stateful)\n   (3 operandi: chiave valore indice); chiave, valore e indice devono essere\n   separati da ':' e tutti sulla stessa linea, ad esempio\n   `gatto:mammifero peloso e affettuoso:3`\nd: cancella un elemento da un dizionario (operazione stateful)\n   (2 operandi: chiave indice);\ne: cerca (2 operandi: chiave indice); \nf: stampa il contenuto di un registro (1 operando: indice);\ng: stampa tutti i registri (0 operandi);\np: mostra questo menu;\nq: esci.\n";
const string prompt = "\ndigitare p per stampare il menu, q per terminare\n> ";

// funzioni di utilità

void check_cin(string err_msg)
{
   // se c'è un errore su cin, resetta l'errore e lancia un'eccezione
   if (!cin) {
      cin.clear();
      cin.ignore(255, '\n');
      throw runtime_error(err_msg);
   }
}

int get_reg_id()
{ // legge un registro da standard input
   int reg;
   const string err_msg = "indice di registro non valido";
   cin >> reg;
   check_cin(err_msg);
   if (reg < 0 || reg >= maxreg)
      throw runtime_error(err_msg);
   return reg;
}

string input_until(char delim)
{
   string res;
   getline(cin, res, delim);
   check_cin("");
   return res;
}

int main(int argc, char *argv[])
{
   bool quiet = argc >= 2 && argv[1][0] == 'q';

   vector<dict::Dictionary> v(maxreg);
   for (int j=0; j<maxreg; ++j)
      v[j] = dict::createEmptyDict();

   if (!quiet)
      cout << menu;

   while (true) {
      if (!quiet)
         cout << prompt;
      char ch;
      cin >> ch;
      if (!cin || ch == 'q')
         break;

      string nome_file;
      dict::Elem e;
      dict::Key key;
      dict::Value value;
      dict::Error error;
      int reg;

      try {
         // parse arguments
         switch (ch) {
            case 'a':  // file_name register
               cin >> nome_file;
               check_cin("nome file non valido");
               reg = get_reg_id();
               break;

            case 'b':
            case 'f':  // reg_id
               reg = get_reg_id();
               break;

            case 'c':  // key:value:register
               key = input_until(':');
               value = input_until(':');
               reg = get_reg_id();
               removeBlanksAndLower(key);
               break;
            
            case 'd':
            case 'e':  // key register
               cin >> key;
               check_cin("errore nella lettura della chiave");
               reg = get_reg_id();
               removeBlanksAndLower(key);
               break;
         }
      }
      catch (runtime_error &e) {
         cerr << "errore nella lettura dei parametri: " << e.what() << endl;
         continue;
      }

      high_resolution_clock::time_point t1 = high_resolution_clock::now();

      try{
         // run commands
         switch (ch) {
            case 'a':
               v[reg] = readFromFile(nome_file);
               break;
            
            case 'b':
               v[reg] = readFromStdin();
               break;
            
            case 'c':
               error = dict::insertElem(key, value, v[reg]);
               break;
            
            case 'd':
               error = dict::deleteElem(key, v[reg]);
               break;
            
            case 'e':
               value = dict::search(key, v[reg]);
               break;
            
            case 'f':
               print(v[reg]);
               cout << endl;
               break;
            
            case 'g':
               for (int i = 0; i < maxreg; i++) {
                  cout << "registro " << i << ":" << endl;
                  print(v[i]);
                  cout << endl;
               }
               break;
            
            case 'p':
               cout << menu;
         }
      }
      catch (runtime_error &e) {
         cerr << "errore nell'esecuzione del comando: " << e.what() << endl;
      }

      high_resolution_clock::time_point t2 = high_resolution_clock::now();

      //output
      switch (ch) {
         case 'c':
         case 'd':
            if (error == dict::OK)
               cout << "operazione riuscita" << endl;
            else
               cout << "operazione fallita" << endl;
            break;

         case 'e':
            if (value != dict::emptyValue)
               cout << "risultato: " << endl << value << endl;
            else
               cout << "elemento non trovato" << endl;
            break;
      }

      cout << "L'operazione ha richiesto " << duration_cast<microseconds>(t2 - t1).count() << " microsecondi" << endl;

   }
}
