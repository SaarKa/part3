//#include "symbol_table.hpp"
#include "part3_helpers.hpp"

extern Code code;

//TypeScopeTable implementation
TypeScopeTable::TypeScopeTable(idTypes type): _curTempOffset((type == INT_t) ? SAVED_REGS_INT : SAVED_REGS_FLOAT),
        _tempStartIndex((type == INT_t) ? SAVED_REGS_INT : SAVED_REGS_FLOAT), _type(type){
    _typeId = (type == INT_t) ? "I" : "F";            
}

string TypeScopeTable::tempReg(){
    return _typeId + to_string(_curTempOffset++);
}


void TypeScopeTable::newReg(string id, int offset){
    _varEntries.insert(pair<string,int>(id,offset));
}


bool TypeScopeTable::searchEntry(varEntry& var, string id){
    auto it = _varEntries.find(id);
    if(it != _varEntries.end()){
        var.type = _type;
        var.place = it->second;
        return true;
    }
    return false;
}

void TypeScopeTable::rmTempReg(){_curTempOffset =_tempStartIndex;}


void TypeScopeTable::emitStore(){
    int regs_num = _curTempOffset - _tempStartIndex;
    if(regs_num > 0){
        string regSP = SP;
        if(_type == FLOAT_t){
            regSP = tempReg();
            code.emit("CITOF",regSP, SP);
        }
    for (int i=0; i<regs_num; i++){
        code.emit(string("STOR")+_typeId + " " +  _typeId + to_string(_tempStartIndex + i) + " " + SP + " " + to_string(i * VAR_SIZE));
    }
    code.emit(string("ADD2I ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));
}

void TypeScopeTable::emitLoad(){
    int regs_num = _curTempOffset - _tempStartIndex;
    if(regs_num > 0){
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));  
        string regSP = SP;
        if(_type == FLOAT_t){
            regSP = tempReg();
            code.emit("CITOF",regSP, SP);
        }
        for (int i=0; i<regs_num; i++){
            code.emit(string("LOAD")+_typeLetter + " " +  _typeLetter + to_string(_tempStartIndex + i) + " " + regSP + " " + to_string(i * VAR_SIZE));
        }
    }
}

/*
void
TypeScopeTable::freeStack(){
    code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(_varEntries.size() * VAR_SIZE));
}
*/
int TypeScopeTable::getVarCount(){return _varOffset - startIndex;}




//ScopeTable implementation
ScopeTable::ScopeTable(int startOffset):intTable(INT_t), floatTable(FLOAT_t),  _varOffset(startOffset), _varStartingIndex(startOffset){}

//ScopeTable::ScopeTable(ScopeTable& oldTable): intTable(oldTable.intTable.getLastVar(), INT_t), floatTable(oldTable.floatTable.getLastVar(), FLOAT_t){}

string ScopeTable::tempReg(idTypes type){
    if(type == VOID_t)
        return "-1";
    return (type == INT_t) ? intTable.tempReg() : floatTable.tempReg();
}

string ScopeTable::newReg(string id, idTypes type){
    if(type == VOID_t)
        return "-1";
    (type == INT_t) ? intTable.newReg(id, _varOffset) : floatTable.newReg(id, _varOffset);
    _varOffset+=VAR_SIZE;
    return _varOffset;
}

bool ScopeTable::searchEntry(varEntry& var, string id){
    assert(!(intTable.searchEntry(var, id) && floatTable.searchEntry(var, id)));
    return intTable.searchEntry(var, id) || floatTable.searchEntry(var, id);
}

void ScopeTable::rmTempReg(){
    intTable.rmTempReg();
    floatTable.rmTempReg();
}

void ScopeTable::storeIds(){
    intTable.emitStore();
    floatTable.emitStore();
}

void ScopeTable::loadIds(){
    intTable.emitLoad();
    floatTable.emitLoad();
}

ScopeTable::freeStack(){
    if(_varOffset > _startIndex){
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(_varOffset));
    }
}

int ScopeTable::getCurOffset(){
    return _varOffset;
}









/*
 // Emit ASM code line to store regs in given range. Update $SP to next available block.
void emitStore(int firstReg, int lastReg, idTypes table_type){
    string type = (table_type == INT_t) ? "I" : "F";
    int regs_num = lastReg - firstReg + 1;
    //Store registers
    for (int i=0; i<regs_num; i++){
        code.emit(string("STOR")+type, type+to_string(i), $SP, to_string(i));
    }
    //Update $SP
    code.emit("ADD2I", $SP, $SP, regs_num);
}

void ScopeTable::storeIds(){
    emitStore(SAVED_REGS_INT, intTable.getLastVar(), INT_t);
    emitStore(SAVED_REGS_FLOAT, floatTable.getLastVar(), FLOAT_t);
    emitStore(intTable.getLastTmp(), intTable.getRegCount()-1, INT_t);
    emitStore(floatTable.getLastTmp(), floatTable.getRegCount()-1, FLOAT_t);
}


//Emit ASM code line to load regs in given range. Update $SP to next available block.
void emitLoad(int firstReg, int lastReg, idTypes table_type){
    string type = (table_type == INT_t) ? "I" : "F";
    int regs_num = lastReg - firstReg + 1;
    //Update $SP
    code.emit("SUBTI", $SP, $SP, regs_num);
    //Load registers
    for (int i=0; i<regs_num; i++){
        code.emit(string("LOAD")+type, type+to_string(i), $SP, to_string(i));
    }
}


void 
ScopeTable::loadIds(){
    emitLoad(floatTable.getLastTmp(), floatTable.getRegCount()-1, FLOAT_t);
    emitLoad(intTable.getLastTmp(), intTable.getRegCount()-1, INT_t);
    emitLoad(SAVED_REGS_FLOAT, floatTable.getLastVar(), FLOAT_t);
    emitLoad(SAVED_REGS_INT, intTable.getLastVar(), INT_t);
}
*/









// VarsTable implementations

void VarsTable::push(){
    int stackOffset = (_tables.size() > 0) ? _tables.front().getCurOffset() : 0; 
    VarScopeTable newTable(stackOffset);
    _tables.push_front(newTable);
}

void VarsTable::pop(){
    _tables.front().freeStack();
    _tables.pop_front();
    if(_tables.size())
        _tables.front().rmTempReg();
}

ScopeTable& VarsTable::head(){return _tables.front();}

bool VarsTable::searchAllTables(varEntry& var, string id){
    for (auto it = _tables.begin(); it != _tables.end(); it++){
        if(it->lookup(var, id))
            return true;
    }
    return false;
}

void VarsTable::setFunctionApi(list<ArgDeclar> &args){argsList = args;}

void VarsTable::storeIds(){_tables.front().storeIds();}

void VarsTable::loadIds(){_tables.front().loadIds();}

bool VarsTable::isInScope(string id){return front().searchEntry(varEntry(),id);}




 //funcEntry implementation
funcEntry::funcEntry(funcProp& funcProps) : _defLine(-1) , props(funcProps) {}

string funcEntry::getDefPlace(int curLine){
    if(!isDefined()){
        callList.push_back(curLine);
    }
    return to_string(_defLine);
}

void funcEntry::define(int defLine){
    _defLine = defLine;
    code.backpatch(callList, defLine);
}

bool funcEntry::isDefined(){return (_defLine != -1);}

bool funcEntry::matchExisting(funcProp& funcProps){
    if(funcProps.type != props.type || funcProps.id != props.id || funcProps.args.size() != props.args.size())
        return false;
    
    for(auto myIt = props.args.begin(), otherIt = funcProps.args.begin();myIt != props.args.end(); myIt++,otherIt++){
        if(myIt->type != otherIt->type)
            return false;
    }

    return true;
}





//FuncTable implemetation
funcEntry* FuncTable::findEntry(string funcName){
    auto it = _FuncTable.findEntry(funcName);
    if(it != _FuncTable.end()) 
        return &it->second;
    return nullptr;
}

funcEntry* FuncTable::addFunc(FunctionProps& funcProps){
    funcEntry newFuncEntry(funcProps);
    _FuncTable.addFunc( pair<string,funcEntry>(funcProps.id, newFuncEntry));
    auto it = _FuncTable.findEntry(funcProps.id);
    assert(it != _FuncTable.end());
    return &it->second;
}

funcEntry* FuncTable::getCurrFunc(){return _current;}

void FuncTable::setCurrFunc(funcEntry *funcEntry){_current = funcEntry;}

string FuncTable::getUnimplementedCalls(){
    string str;
    for (auto const& funcIt : _FuncTable){
        funcEntry func = funcIt.second;
        str += " ";
        str += func.props.id;
        for(int line : func.callList){
            str += ",";
            str += to_string(line);
        }
    }
    return str;
}

string FuncTable::getImplemented(){
    string str;
    for (auto const& funcIt : _FuncTable){
        funcEntry func = funcIt.second;
        if(func.isDefined()){
            str += " ";
            str += func.props.id;
            str += ",";
            str += func.getDefPlace(0);
        }
    }
    return str;
}