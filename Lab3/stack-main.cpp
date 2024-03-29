#include "stack-ptr.h" 


const string menu = "Avete a disposizione 10 registri numerati da 0 a 9 che contengono degli stack\ninizialmente vuoti.\nInserite l'operazione scelta e di seguito gli operandi richiesti.\na: leggi gli elementi di uno stack (con inserimento in cima) da file terminato\n   da -1000000 (2 operandi: nome_file indice);\nb: leggi gli elementi di uno stack (con inserimento in cima) da standard input\n   (1 operando: indice);\nc: push (2 operandi: elem indice);\nd: pop (1 operando: indice);\ne: top (1 operando: indice); \nf: isEmpty (1 operando: indice); \nn: stampa (1 operando: indice);\no: stampa tutti i registri (0 operandi);\np: mostra questo menu;\nq: esci.\n";
const string prompt = "\ndigitare p per stampare il menu, q per terminare\n> ";

const int MAXREG = 10;

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

   vector<stack::Stack> v(MAXREG);
   for (int i=0; i<MAXREG; ++i)
      v[i] = stack::EMPTYSTACK;

   if (!quiet)
      cout << menu;

   while (true) {
      if (!quiet)
         cout << prompt;
      char ch;
      cin >> ch;
      if (!cin || ch == 'q')
         break;
      try {
         string nome_file;
         stack::Elem e;
         switch(ch) {
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
               stack::push(e, v[get_reg_id()]);
               break;

            case 'd':
               e = stack::pop(v[get_reg_id()]);
               if (e == stack::EMPTYELEM)
                  msg("stack vuoto");
               else
                  msg("elemento rimosso: " + to_string(e));
               break;

            case 'e':
               e = stack::top(v[get_reg_id()]);
               if (e == stack::EMPTYELEM)
                  msg("stack vuoto");
               else
                  msg("elemento in cima allo stack: " + to_string(e));
               break;

            case 'f':
               if (stack::isEmpty(v[get_reg_id()]))
                  msg("stack vuoto");
               else
                  msg("stack non vuoto");
               break;

            case 'n':
               print(v[get_reg_id()]);
               break;

            case 'o':
               for (int i=0; i<MAXREG; ++i) {
                  cout << "stack " << i << ": ";
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
