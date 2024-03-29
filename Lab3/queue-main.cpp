#include "queue-array.h" 

const string menu = "Avete a disposizione 10 registri numerati da 0 a 9 che contengono code\ninizialmente vuote.\nInserite l'operazione scelta e di seguito gli operandi richiesti.\na: leggi gli elementi di una coda (con inserimento in fondo) da file terminato\n   da -1000000 (2 operandi: nome_file indice);\nb: leggi gli elementi di una coda (con inserimento in fondo) da standard input\n   (1 operando: indice);\nc: enqueue (2 operandi: elem indice);\nd: dequeue (1 operando: indice);\ne: first (1 operando: indice); \nf: isEmpty (1 operando: indice); \nn: stampa (1 operando: indice);\no: stampa di tutti i registri (0 operandi);\np: mostra questo menu;\nq: esci.\n";
const string prompt = "\ndigitare p per stampare il menu, q per terminare\n> ";

const int MAXREG = 10;

// funzioni di utilità

void check_cin(string err_msg) {
   // se c'è un errore su cin, resetta l'errore e lancia un'eccezione
   if (!cin) {
      cin.clear();
      cin.ignore(255, '\n');
      throw runtime_error(err_msg);
   }
}

int get_reg_id() { // legge un registro da standard input
   int reg;
   const string err_msg = "indice di registro non valido";
   cin >> reg;
   check_cin(err_msg);
   if (reg < 0 || reg >= MAXREG)
      throw runtime_error(err_msg);
   return reg;
}

void msg(string m) { // scrive un messaggio su standard output
   cout << m << endl;
}

int main(int argc, char* argv[]) {
   bool quiet = argc >= 2 && argv[1][0] == 'q';

   vector<queue::Queue> v(MAXREG);
   for (int i=0; i<MAXREG; ++i)
      v[i] = queue::createEmpty();

   if (!quiet)
      cout << menu;

   while(true) {
      if (!quiet)
         cout << prompt;
      char ch;
      cin >> ch;
      if (!cin || ch == 'q')
         break;
      try {
         string nome_file;
         queue::Elem e;
         switch (ch) {
            case 'a':
               cin >> nome_file;
               check_cin("nome file non valido");
               v[get_reg_id()] = readFromFile(nome_file);
               break;

            case 'b':
               v[get_reg_id()] = readFromStdin();
               break;
            
            case 'c':
               cin >> e;
               check_cin("elemento non valido");
               queue::enqueue(e, v[get_reg_id()]);
               break;

            case 'd':
               e = queue::dequeue(v[get_reg_id()]);
               if (e == queue::EMPTYELEM)
                  msg("coda vuota");
               else
                  msg("elemento rimosso: " + to_string(e));
               break;

            case 'e':
               e = queue::first(v[get_reg_id()]);
               if (e == queue::EMPTYELEM)
                  msg("coda vuota");
               else
                  msg("elemento in testa alla coda: " + to_string(e));
               break;

            case 'f':
               if (queue::isEmpty(v[get_reg_id()]))
                  msg("coda vuota");
               else
                  msg("coda non vuota");
               break;

            case 'n':
               print(v[get_reg_id()]);
               break;
            
            case 'o':
               for (int i=0; i<MAXREG; ++i) {
                  cout << "coda " << i << ": ";
                  print(v[i]);
               }
               break;

            case 'p': 
               cout << menu;
               break;

            default:
               throw runtime_error("opzione non valida");
         }
      }
      catch (runtime_error &e) {
         cerr << "errore a runtime: " << e.what() << endl;
      }

   }
}
