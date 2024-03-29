#include "string-utility.h"

void stringLower(string& s)
{
   for (string::size_type i=0; i<s.length(); ++i)
      s[i] = tolower(s[i]); 
}

void normalize(string& s)
{
   s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
   stringLower(s);   
}
