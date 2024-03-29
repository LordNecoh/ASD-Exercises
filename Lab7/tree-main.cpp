// Compilare con g++ -Wall -std=c++11 *.cpp -o tree 
 
#include "tree-chsib.h"

const int maxreg = 10;
const string menu = R"(Hai a disposizione 10 registri numerati da 0 a 9, che contengono alberi	 
inizialmente vuoti. 
Inserisci l'operazione scelta e di seguito gli operandi richiesti.
Nota: tutti i whitespace (spazi, newline, tab, etc.) verranno rimossi dalle 
etichette; l'etichetta vuota è rappresentata da "$#$#$".

a: leggi un albero da file [la prima riga del file deve contenere solo
   l'etichetta della radice; le righe successive devono rispettare la
   sintassi labelPadre labelFiglio1 labelFiglio2 .... labelFiglioN]
   (2 operandi: nome_file indice);
b: inserisci un nodo etichettato labelFiglio attaccato a un padre etichettato
   labelPadre (3 operandi: labelPadre labelFiglio indice); se labelPadre è
   "$#$#$", il nodo verrà inserito come radice se l'albero è vuoto;
c: cancella un nodo etichettato label dall'albero; se `modo` è 'r' verrà
   usata l'implementazione ricorsiva, se è 'i' quella iterativa;
   (3 operandi: label indice modo);
d: individua il padre del nodo etichettato label (2 operandi: label indice);
e: individua le etichette dei figli del nodo etichettato label
   (2 operandi: label indice);
f: calcola il grado del nodo con etichetta label (2 operandi: label indice);
g: individua le etichette degli antenati del nodo etichettato label; se `modo`
   è 'r' verrà usata l'implementazione ricorsiva, se è 'i' quella iterativa;
   (3 operandi: label indice modo);
h: individua l'etichetta del minimo antenato comune a due nodi etichettati
   label1 e label2 (3 operandi: label1 label2   indice);
i: verifica se un nodo con etichetta label appartiene all'albero
   (2 operandi: label   indice);
l: calcola il numero totale dei nodi dell'albero (1 operando: indice);
m: stampa un registro (1 operando: indice);
n: stampa tutti i registri (0 operandi);
p: mostra questo menu; 
q: termina.
)";
const string prompt = "\ndigita p per stampare il menu, q per terminare\n> ";

// funzioni di utilità

// se c'è un errore su cin, resetta l'errore e lancia un'eccezione
void check_cin(string err_msg)
{
   if (!cin) {
      cin.clear();
      cin.ignore(255, '\n');
      throw runtime_error(err_msg);
   }
}

// legge un registro da standard input
int get_reg_id()
{ 
   int reg;
   const string err_msg = "indice di registro non valido";
   cin >> reg;
   check_cin(err_msg);
   if (reg < 0 || reg >= maxreg)
      throw runtime_error(err_msg);
   return reg;
}

string get_label()
{
   string res;
   cin >> res;
   check_cin("etichetta non valida");
   normalize(res);  // rimuovo whitespace e converto a minuscolo
   return res;
}

bool get_recursive()
{
   char ch;
   cin >> ch;
   check_cin("modo non valido");
   if (ch == 'r')
      return true;
   else if (ch == 'i')
      return false;
   else
      throw runtime_error("modo non valido");
}

int main(int argc, char *argv[])
{
   bool quiet = argc >= 2 && argv[1][0] == 'q';

   vector<tree::Tree> v(maxreg);
   for (int i=0; i<maxreg; ++i)
      v[i] = tree::createEmpty();

   if (!quiet)
      cout << menu;

   while (true) {
      if (!quiet)
         cout << prompt;
      char ch;
      cin >> ch;
      if (!cin || ch == 'q')
         break;

      bool recursive;
      int reg;
      string nome_file, label, label2;
      
      try {  //parse arguments
         switch(ch) {
            case 'a': // file_name register
               cin >> nome_file;
               check_cin("nome file non valido");
               reg = get_reg_id();
               break;
         
            case 'b':
            case 'h': // label label2 register
               label = get_label();
               label2 = get_label();
               reg = get_reg_id();
               break;

            case 'd':
            case 'e':
            case 'f':
            case 'i': // label register
               label = get_label();
               reg = get_reg_id();
               break;
            
            case 'c':
            case 'g': // label register mode
               label = get_label();
               reg = get_reg_id();
               recursive = get_recursive();
               break;

            case 'l':
            case 'm': // register
               reg = get_reg_id();
               break;
            
            case 'n':
            case 'p': // no operands
               break;
            
            default:
               throw runtime_error("comando sconosciuto");
         } // switch
      } // try
      catch (runtime_error &e) {
         cerr << "errore nella lettura dei parametri: " << e.what() << endl;
         continue;
      }

      high_resolution_clock::time_point t1 = high_resolution_clock::now();
      tree::Error error;
      string result;
      list::List results;

      try { // run commands
         switch(ch) {
            case 'a':
               v[reg] = readFromFile(nome_file);
               break;
            
            case 'b':
               cerr << "adding " << label2 << " child of " << label << endl;
               error = tree::addElem(label, label2, v[reg]);
               break;
            
            case 'c':
               error = recursive ?
                  tree::deleteElemR(label, v[reg]) :
                  tree::deleteElemI(label, v[reg]);
               break;
            
            case 'd':
               result = tree::father(label, v[reg]);
               break;
            
            case 'e':
               results = tree::children(label, v[reg]);
               break;
            
            case 'f':
               result = to_string(tree::degree(label, v[reg]));
               break;
            
            case 'g':
               results = recursive ?
                  tree::ancestorsR(label, v[reg]) :
                  tree::ancestorsI(label, v[reg]);
               break;
            
            case 'h':
               result = tree::leastCommonAncestor(label, label2, v[reg]);
               break;
            
            case 'i':
               result = tree::member(label, v[reg]) ? "vero" : "falso";
               break;
            
            case 'l':
               result = to_string(numNodes(v[reg]));
               break;
            
            case 'm':
               printTree(v[reg]);
               break;

            case 'n':
               for (int i=0; i < maxreg; i++) {
                  cout << "Albero " << i << ":" << endl;
                  printTree(v[i]);
               }
               break;

            case 'p':
               cout << menu;
               break;            
         }  // switch
      } // try
      catch (runtime_error &e) {
         cerr << "errore nell'esecuzione del comando: " << e.what() << endl;
      }

      high_resolution_clock::time_point t2 = high_resolution_clock::now();
     
      cout << "L'operazione ha richiesto " <<
         duration_cast<microseconds>(t2 - t1).count() << " microsecondi"
         << endl;

      switch(ch) {  // eventuale output
         case 'b':
         case 'c':
            cout << "operazione " << (error == tree::OK ? "riuscita" : "fallita")
               << endl;
            break;
         
         case 'd':
         case 'f':
         case 'h':
         case 'i':
         case 'l':
            cout << result << endl;
            break;
         
         case 'e':
         case 'g':
            printList(results);
            break;
      }  // switch
   }  // while
}
