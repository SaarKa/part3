//#include <stdio.h>
#include "part3_helpers.hpp"
//#include "symbol_table.hpp"
//#include "code_class_helpers.hpp"
using namespace std;

extern ParsNode *parseTree;
extern Code code;
extern FuncTable funcTable;
extern int yyparse (void);

string_code RelopHit (string const& str) {
    if (str == "==") return Eq;
    if (str == "<>") return Neq;
    if (str == "<") return Lt;
    if (str == "<=") return LtEq;
    if (str == ">") return Gt;
    if (str == ">=") return GtEq;
    return Default;
}

string_code ArithHit (string const& str) {
    if (str == "+") return Add;
    if (str == "-") return Sub;
    if (str == "*") return Mul;
    if (str == "/") return Div;
    return Default;
}

/**************************************************************************/
/*                 implementation of the classes functions                */
/**************************************************************************/
//ParsNode implementation
ParsNode::ParsNode(string type) : type(type) {}
ParsNode::ParsNode(string type, ParsNode *sibling) : type(type), sibling(sibling) {}
ParsNode::~ParsNode(){
  if(sibling != nullptr)
    delete sibling;
}
void ParsNode::dumpParseTree(){
  dumpParseTree(0);
}


//SymNode implementation
//SymNode::SymNode(string type, ParsNode *child) : ParsNode(type), child(child) {}
SymNode::~SymNode(){
  delete child;
}

void SymNode::dumpParseTree(int depth){
  string indent(depth * 2,' ');
  cout << indent << "(<" << type << ">" << endl;
  for (ParsNode* curChild = child; curChild != NULL; curChild = curChild->sibling) {
    curChild->dumpParseTree(depth + 1);
  }
  cout << indent << ")" << endl;
}

//TokenNode implementation
/*
TokenNode::TokenNode(string type) : ParsNode(type) {
  cout << "token: " << type << endl;
}
TokenNode::TokenNode(string type, string value) : ParsNode(type), value(value){
  cout << "token: " << type << ":" << value << endl;
}
*/

TokenNode::TokenNode(string type) : ParsNode(type) {}

TokenNode::TokenNode(string type, string value) : ParsNode(type), value(value){}

void TokenNode::dumpParseTree(int depth){
  string indent(depth * 2,' ');
  if(value.length() > 0)
    cout << indent << "(<" << type << "," << value << ">)" << endl;
  else
    cout << indent << "(<" << type << ">)" << endl;
}



//modified conact function implementation
ParsNode *concatList(ParsNode *head,ParsNode *newItm)
{
  ParsNode* lastSib;

  if (head == NULL) return newItm;
  lastSib = head;
  while (lastSib->sibling != (ParsNode*)NULL)
    lastSib = lastSib->sibling; 
  lastSib->sibling = newItm;
  return (head);
}


void opError(const char* error){
    cerr << "opError: " << error <<endl;
    exit(9);
}

//modified main function implementation for the parser
int main (int argc, char **argv) {
    int rc;
    try{
      extern FILE *yyin;
      yyin = fopen(argv[1], "r");
      if (yyin == NULL) {
        opError("can't open input file");
      }
      rc = yyparse();
      if (rc == 0) {
          ofstream outFile;
          string outputName = argv[1];
          bool replaceName = replace(outputName, ".cmm", ".rsk");
          if(!replaceName){
            opError("invalid file name. must end with .cmm");
          }
          outFile.open(outputName); 
          //Print headers (for linker)
          outFile << "<header>" << endl;
          outFile << "<unimplemented>" << funcTable.getUnimplementedCalls() << endl;
          outFile << "<implemented>" << funcTable.getImplemented() << endl;
          outFile << "</header>" << endl;
          //Print code
          code.print(outFile);
          outFile.close();
      }
    } catch (exception e){
      opError(e.what());
    }
    delete parseTree;
    return rc;
}

