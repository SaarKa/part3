//#include <stdio.h>
#include "part3_helpers.hpp"
//#include "symbol_table.hpp"
//#include "code_class_helpers.hpp"
using namespace std;

extern ParsNode *parseTree; /* Root of parse tree. Defined in the parser. */
extern Code code;
extern FuncTable funcTable;
extern int yyparse (void);




string_code RelopHit (string const& inString) {
    if (inString == "<") return Lt;
    if (inString == ">") return Gt;
    if (inString == ">=") return GtEq;
    if (inString == "==") return Eq;
    if (inString == "<>") return Neq;
    if (inString == "<=") return LtEq;
    return Default;
}

string_code ArithHit (string const& inString) {
    if (inString == "*") return Mul;
    if (inString == "/") return Div;
    if (inString == "+") return Add;
    if (inString == "-") return Sub;
    return Default;
}

/**************************************************************************/
/*                 implementation of the classes functions                */
/**************************************************************************/
//ParsNode implementation
ParsNode::ParsNode(string type):type(type){}
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
  string indent(depth*2,' ');
  cout << indent << "(<" << type << ">" << endl;
  for (ParsNode* curChild = child; curChild != NULL; curChild = curChild->sibling) {
    curChild->dumpParseTree(depth + 1);
  }
  cout << indent << ")" << endl;
}

/*
//TokenNode implementation
TokenNode::TokenNode(string type) : ParsNode(type) {
  cout << "token: " << type << endl;
}
TokenNode::TokenNode(string type, string value) : ParsNode(type), value(value){
  cout << "token: " << type << ":" << value << endl;
}
*/

void TokenNode::dumpParseTree(int depth){
  string indent(depth * 2,' ');
  if(value.length() > 0)
    cout << indent << "(<" << type << "," << value << ">)" << endl;
  else
    cout << indent << "(<" << type << ">)" << endl;
}

/*
//DeclarNode implementation
DeclarNode::DeclarNode(string type, ParsNode* child, idTypes idType, string id): SymNode(type, child), idType(idType) {
  idList.push_back(id);
}

DeclarNode::DeclarNode(string type, ParsNode* child, string id, DeclarNode prevDeclare): SymNode(type, child), idType(prevDeclare.idType) {
   idList = prevDeclare.idList;
   idList.push_back(id);
}
*/


//modified conact function implementation
ParsNode *concatList(ParsNode *head,ParsNode *newItem)
{
  ParsNode* lastSibling;
  if (head == NULL) {return newItem};
  lastSibling = head;
  while (lastSibling->sibling != (ParsNode*)NULL){
    lastSibling = lastSibling->sibling;
  }
  lastSibling->sibling = newItem;
  return (head);
}


//modified main function implementation for the parser
int main (int argc, char **argv) {
    int rc;
    try{
      extern FILE *yyin;
      yyin = fopen(argv[1], "r");
      if (yyin == NULL) {
        op_error("can't open input file");
      }
      rc = yyparse();
      if (rc == 0) {
          ofstream outFile;
          string outputName = argv[1];
          bool replaceName = replace(outputName, ".cmm", ".rsk");
          if(!replaceName){
            op_error("wrong file name.");
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
      op_error(e.what());
    }
    delete parseTree;
    return rc;
}


//operation error
void op_error(const char* error){
    printf("po error: %s",error);
    exit(9);
}