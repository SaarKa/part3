#ifndef COMMON_H
#define COMMON_H
//#include "code_class_helpers.hpp"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include <cassert>
#include <exception>
using namespace std;

#define YYSTYPE ParsNode*
#define PRINT_PARSE_TREE 1
#define FUNC_ARGS_OFFSET 1
#define SAVED_REGS_INT 5
#define SAVED_REGS_FLOAT 1
#define VAR_SIZE 4

#define $RA "I0" //ret address
#define $FP "I1" //frame ptr
#define $SP "I2" //stack ptr
#define $RI "I3" //ret of type int
#define $DP "I4" //declaration ptr
#define $RF "F0" //ret of type float

enum idTypes {INT_t, FLOAT_t, VOID_t};

bool replace(string& str, const string& from, const string& to);

typedef struct _ArgDeclar {
    string id;
    idTypes type;
} ArgDeclar;

typedef struct _funcProp {
    idTypes type;
    string id;
    list<ArgDeclar> args;
} funcProp;

//general error
void genErr(const char* error);

//the lists needed for backpatching. line: the first line to insert
class CodeList : public list<int> {
public:
    CodeList();
    CodeList(int line);
};

class Code {
protected:

    vector<string> codeVector;
    template<typename T>
    string typeString(T str) {
        return str;
    }

    template<typename T, typename... Args>
    string typeString(T t, Args... args){
        return typeString(t) + string(" ") + typeString(args...);
    };


public:
    Code();
    //emit code and return line number where the code is emitted
    //done using template to be used with all type of args
    template<typename... Args>
    int emit(Args... args){
        string code = typeString(args...);
        codeVector.push_back(code);
        return codeVector.size();
    }


    //return the next line to write code to
    int nextQuad();

    //backpatch all lines in list with lineNumber
    void backpatch(CodeList list, int lineNumber);

    //print full code
    void print(ofstream& file);
};



//general node, all types of node inherit from this class
class ParsNode{
public:
    string type;
    ParsNode *sibling;
    //constructors
    ParsNode(){}
    ParsNode(string type);
    ParsNode(string type, ParsNode *sibling);
    //destructor
    ~ParsNode();

    void dumpParseTree();
    virtual void dumpParseTree(int depth) = 0;
};



//modified concate function to c++
ParsNode *concatList(ParsNode *head,ParsNode *item);



//symbol node. inherit from ParsNode 
class SymNode : public ParsNode{
    ParsNode *child;
public:
    //SymNode(string type, ParsNode *child);
    ~SymNode();
    void dumpParseTree(int depth);
};

class DeclaredFuncArgs{
public:
    string id;
    idTypes type;
    DeclaredFuncArgs(string id, idTypes type) : id(id), type(type){}
};

//token node. inherit from ParsNode 
class TokenNode : public ParsNode{
public:
    string value;
    TokenNode(string type);
    TokenNode(string type, string value);
    virtual void dumpParseTree(int depth);
};

#define EPSILON new TokenNode("EPSILON")


//Nodes that inherit from SymNode 
class ExpNode : public SymNode{
public:
    idTypes exp;
    string place;
};

class TypeNode : public SymNode{
public:
    idTypes typeVal;
};


class StmtNode : public SymNode{
public:
    CodeList nextlist;
    CodeList breaklist;
};

class BranchExpNode : public SymNode{
public:
    CodeList truelist;
    CodeList falselist;
};

class DeclarNode : public SymNode{
public:
    list<string> idList;
    idTypes idType;
};

class FuncApiNode : public SymNode{
public:
    funcProp funcProps;
};

class CallArgsNode : public SymNode{
public:
    list<ExpNode> expList;
};

class ArgLstNode : public SymNode{
public:
    list<ArgDeclar> argsList;
};

class MMarkNode : public SymNode{
public:
    string quad;
};

class NMarkNode : public SymNode{
public:
    CodeList nextlist;
};

//string comparison analysis

enum str_code {Lt, LtEq, Gt, GtEq, Eq, Neq, Mul, Div, Add, Sub, Default}


class varEntry{
public:
    int place;
    idTypes type;
};

string_code RelopHit (string const& inString);

string_code ArithHit (string const& inString);

class TypeScopeTable{
protected:
    int _varOffset;
    int _tempStartIndex;
    idTypes _type;
    string _typeId;
    map<string,int> _varEntries;

public:
    //constructor
    TypeScopeTable(idTypes type);
    //return ptr to the allocated temporary var place
    string tempReg();

    //return chosen register of the var
    string newReg(string id, int offset);

    bool searchEntry(varEntry& var, string id);

    void rmTempReg();

    //return number of registers in use
    //int getRegCount();

    //Emit ASM code line to load regs in given range. Update $SP to next available block
    void emitLoad();

    // Emit ASM code line to store regs in given range. Update $SP to next available block.
    void emitStore();

    //remove vars
    //void freeStack();
    int getVarCount();
};


class ScopeTable{
protected:
    int _varOffset;
    int _startIndex;
    TypeScopeTable intTable;
    TypeScopeTable floatTable;
public:
    //consructors
    ScopeTable(int startOffset);
    //ScopeTable(ScopeTable& oldTable);

    //return new temporary register
    string tempReg(idTypes type)

    //return new register
    string newReg(string id, idTypes type);

    //return true if id exist and update var pointer to the entry. else false.
    bool searchEntry(varEntry& var, string id);

    //remove tempReg from scope
    void rmTempReg();

    //store regs and temp regs to the memory
    void storeIds();

    //load regs and temp regs from the memory
    void loadIds();

    void freeStack();

    int getCurOffset();
};


class VarsTable{
protected:
    list<ScopeTable> _tables;
public:
    list<ArgDeclar> argsList;

    //enter ScopeTable to the list end
    void push();

    //remove the start of the list
    void pop();

    //remove and return the list head
    ScopeTable& head();

    //search closest occurence of var in all ScopeTables.
    //if found, return through pointer and return true, else false
    bool searchAllTables(varEntry& var, string id);

    //brief insert func API to arguments list.
    void setAPI(list<ArgDeclar> &args);
    
    //Execute storeIds to the head of _tables in ScopeTable 
    void storeIds();

    //Execute storeIds to the head of _tables in ScopeTable 
    void loadIds();

    bool isInScope(string id);
};


class funcEntry{
protected:
    int _defLine;
public:
    funcProp props;
    CodeList callList;

    //constructor. gets the props of the func
    funcEntry(funcProp& funcProps);


    //if func defined, return place, else -1
    string getDefPlace(int curLine);

     //Store the beginning of the defenition line.
     //arg is the begining of the defenition line
    void define(int defLine);

    bool isDefined();

    //return true if props match func, else false
    bool matchFunc(funcProp& funcProps);
};



class FuncTable{
protected:
    map<string,funcEntry> _funcTable;
    funcEntry* _curr;
public:
    
     //return ptr to entry if found, else NULL.
    funcEntry* findEntry(string funcName);

    //add new function entry.
    funcEntry* addFunc(funcProp& funcProps);

    //return ptr to the entry of the last func that was inserted
    funcEntry* getCurrFunc();

    //change _curr to be the given funcEntry
    void setCurrFunc(funcEntry* funcEntry);

    string getUnimplementedCalls();

    string getImplemented();
};

#endif
