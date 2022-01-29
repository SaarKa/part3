#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
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
#define SAVED_IREGS 5
#define SAVED_FREGS 4
#define VAR_SIZE 4

#define RA "I0" //ret address
#define FP "I1" //frame ptr
#define SP "I2" //stack ptr
#define RI "I3" //ret of type int
#define DI "I4" //declaration ptr

#define FP_F "F0" //frame ptr
#define SP_F "F1" //stack ptr
#define RF "F2" //ret of type float
#define DF "F3" //declaration ptr

enum idTypes {INT_t = 0, FLOAT_t = 1, VOID_t = 2};
const string idTypesNames[3] = {"int", "float", "void"};

bool replace(string& str, const string& from, const string& to);

typedef struct _ArgDeclr {
    string id;
    idTypes type;
} ArgDeclr;

typedef struct _functionProps {
    idTypes type;
    string id;
    list<ArgDeclr> args;
} functionProps;

//general error
void genErr(const char* error);
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
    string typeString(T t, Args... args) {
        return typeString(t) + string(" ") + typeString(args...) ;
    }

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
	void backpatch(CodeList list, int lineNo);

	//print full code
	void print(ofstream& file);
};

//general node, all types of node inherit from this class
class ParsNode{
public:
    string type;
    ParsNode *sibling;

    ParsNode(){}
    ParsNode(string type);
    ParsNode(string type, ParsNode *sibling);
    //destructor
    virtual ~ParsNode();

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
    virtual void dumpParseTree(int depth);
};


class TokenNode : public ParsNode{
public:
    string value;
    TokenNode(string type);
    TokenNode(string type, string value);
    virtual void dumpParseTree(int depth);
};

#define EPSILON new TokenNode("EPSILON")

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
    functionProps funcProps;
};

class CallArgsNode : public SymNode{
public:
    list<ExpNode> expList;
};

class ArgLstNode : public SymNode{
public:
    list<ArgDeclr> argList;
};

class MMarkNode : public SymNode{
public:
    int quad;
};

class NMarkNode : public SymNode{
public:
    CodeList nextlist;
};


enum str_code {Eq, Neq, Lt, LtEq, Gt, GtEq, Add, Sub, Mul, Div, Default};

str_code RelopHit (string const& str);

str_code ArithHit (string const& str);


/**
 * @brief Concatenate a child to the end of its children list.
 * 
 * @param listHead Pointer to list's head (Its first child)
 * @param newItem Pointer to the new child to be added in the end of the list.
 * @return ParsNode* Pointer to the list's head.
 */
ParsNode *concatList(ParsNode *listHead,ParsNode *newItem);

/**
 * @brief Failed to allocate memory, wrong input etc.
 * 
 * @param err Error message
 */
void opError(const char* err);

class VarEntry{
public:
    int offset;
    idTypes type;
};

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
    void newReg(string id, int offset);
    bool lookup(VarEntry& var, string id);
    void rmTempReg();
    //Emit ASM code line to load regs in given range. Update $SP to next available block
    void emitLoad();
    // Emit ASM code line to store regs in given range. Update $SP to next available block.
    void emitStore();
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
    string tempReg(idTypes type);
    int newReg(string id, idTypes type);
    //return true if id exist and update var pointer to the entry. else false.
    bool lookup(VarEntry& var, string id);
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
    list<ArgDeclr> argsList;
    //enter ScopeTable to the list end
    void push();
    //remove the start of the list
    void pop();
    //remove and return the list head
    ScopeTable& front();
    bool searchAllTables(VarEntry& var, string id);
    //insert func API to arguments list.
    void setAPI(list<ArgDeclr> &args);
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
    functionProps props;
    CodeList callList;
    //constructor. gets the props of the func
    funcEntry(functionProps& funcProps);
    //if func defined, return place, else -1
    string getPlace(int curLine);
    //Store the beginning of the defenition line.
    void define(int _defLine);
    bool isDefined();
    //return true if props match func, else false
    bool matchFunc(functionProps& funcProps);
};

class FuncTable{
protected:
    map<string,funcEntry> _FuncTable;
    funcEntry* _current;
public:
    //return ptr to entry if found, else NULL.
    funcEntry* find(string id);
    //add new function entry
    funcEntry* insert(functionProps& funcProps);
    //return ptr to the entry of the last func that was inserted
    funcEntry* getCurrent();
    //change _current to be the given funcEntry
    void setCurrent(funcEntry* funcEntry);
    string getUnimplementedCalls();
    string getImplemented();

};


#endif
