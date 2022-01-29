//#include "symbol_table.hpp"
#include "part3_helpers.hpp"

extern Code code;


//TypeScopeTable Implementation
TypeScopeTable::TypeScopeTable(idTypes type): _varOffset((type == INT_t) ? SAVED_IREGS : SAVED_FREGS),
        _tempStartIndex((type == INT_t) ? SAVED_IREGS : SAVED_FREGS), 
        _type(type){
    _typeId = (type == INT_t) ? "I" : "F";            
}

string TypeScopeTable::tempReg(){return _typeId + to_string(_varOffset++);}

void TypeScopeTable::newReg(string id, int offset){_varEntries.insert(pair<string,int>(id,offset));}

bool TypeScopeTable::lookup(VarEntry& var, string id){
    auto it = _varEntries.find(id);
    if(it != _varEntries.end()){
        var.type = _type;
        var.offset = it->second;
        return true;
    }
    return false;
}

void TypeScopeTable::rmTempReg(){_varOffset = _tempStartIndex;}

void TypeScopeTable::emitStore(){
    int regs_num = _varOffset - _tempStartIndex;
    string regSP = SP;
    if(regs_num > 0){
        if(_type == FLOAT_t){
            code.emit("CITOF",SP_F, SP);
            regSP = SP_F;
        }
        for (int i=0; i<regs_num; i++){
            code.emit(string("STOR")+_typeId + " " +  _typeId + to_string(_tempStartIndex + i) + " " + regSP + " " + to_string(i * VAR_SIZE));
        }
        code.emit(string("ADD2I ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));
    }
}

void TypeScopeTable::emitLoad(){
    int regs_num = _varOffset - _tempStartIndex;
    if(regs_num > 0){
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));
        string regSP = SP;
        if(_type == FLOAT_t){
            code.emit("CITOF",SP_F, SP);
            regSP = SP_F;
        }
        for (int i=0; i<regs_num; i++){
            code.emit(string("LOAD")+_typeId + " " +  _typeId + to_string(_tempStartIndex + i) + " " + regSP + " " + to_string(i * VAR_SIZE));
        }
    }
}


// ScopeTable Implementations

ScopeTable::ScopeTable(int startOffset) : 
    _varOffset(startOffset), _startIndex(startOffset), intTable(INT_t), floatTable(FLOAT_t) {}

string ScopeTable::tempReg(idTypes type){
    if(type == VOID_t)
        return "-1";
    return (type == INT_t) ? intTable.tempReg() : floatTable.tempReg();
}

int ScopeTable::newReg(string id, idTypes type){
    if(type == VOID_t)
        return -1;
    (type == INT_t) ? intTable.newReg(id, _varOffset) : floatTable.newReg(id, _varOffset);
    _varOffset+=VAR_SIZE;
    return _varOffset;
}

bool ScopeTable::lookup(VarEntry& var, string id){
    bool isInIntTable = intTable.lookup(var, id);
    bool isInFloatTable = floatTable.lookup(var, id);
    assert(!(isInIntTable && isInFloatTable));
    return isInIntTable || isInFloatTable;
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
    floatTable.emitLoad();
    intTable.emitLoad();
}

void ScopeTable::freeStack(){
    if(_varOffset > _startIndex)
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(_varOffset));
}

int ScopeTable::getCurOffset(){return _varOffset;}



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





 //VarsTable implementations
 
void VarsTable::push(){
    int stackOffset = (_tables.size() > 0) ? _tables.front().getCurOffset() : 0; 
    ScopeTable newTable(stackOffset);
    _tables.push_front(newTable);
}

void VarsTable::pop(){
    _tables.front().freeStack();
    _tables.pop_front();
    if(_tables.size())
        _tables.front().rmTempReg();
}

ScopeTable& VarsTable::front(){
    return _tables.front();
}

bool VarsTable::searchAllTables(VarEntry& var, string id){
    for (auto it = _tables.begin(); it != _tables.end(); it++){
        if(it->lookup(var, id))
            return true;
    }
    return false;
}

void VarsTable::setAPI(list<ArgDeclr> &args){
    argsList = args;
}

void VarsTable::storeIds(){_tables.front().storeIds();}

void VarsTable::loadIds(){_tables.front().loadIds();}

bool VarsTable::isInScope(string id){
    VarEntry tmp;
    return front().lookup(tmp,id);
}


//funcEntry implementation
funcEntry::funcEntry(functionProps& funcProps) : _defLine(-1) , props(funcProps) {}

string funcEntry::getPlace(int curLine){
    if(!isDefined()){
        callList.push_back(curLine);
    }
    return to_string(_defLine); //will be "-1" if not defined
}

void funcEntry::define(int _defLine){
    _defLine = _defLine;
    code.backpatch(callList, _defLine);
}

bool funcEntry::isDefined(){
    return (_defLine != -1);
}

bool funcEntry::matchFunc(functionProps& funcProps){
    if(funcProps.type != props.type || funcProps.args.size() != props.args.size() || funcProps.id != props.id)
        return false;
    for(auto myIt = props.args.begin(), otherIt = funcProps.args.begin();myIt != props.args.end(); myIt++,otherIt++){
        if(myIt->type != otherIt->type)
            return false;
    }

    return true;
}


 //FuncTable implemetation
funcEntry* FuncTable::find(string id){
    auto it = _FuncTable.find(id);
    if(it != _FuncTable.end()) //found the function entry
        return &it->second;
    return nullptr;
}

funcEntry* FuncTable::insert(functionProps& funcProps){
    funcEntry newFunc(funcProps);
    _FuncTable.insert( pair<string,funcEntry>(funcProps.id, newFunc));
    auto it = _FuncTable.find(funcProps.id);
    assert(it != _FuncTable.end());
    return &it->second;
}

funcEntry* FuncTable::getCurrent(){return _current;}

void FuncTable::setCurrent(funcEntry *funcEntry){_current = funcEntry;}

string FuncTable::getUnimplementedCalls(){
    string str;
    for (auto const& funcIt : _FuncTable){
        funcEntry func = funcIt.second;
        if(func.callList.size() != 0){
            str += " ";
            str += func.props.id;
            for(int line : func.callList){
                str += ",";
                str += to_string(line);
            }
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
            str += func.getPlace(0);
        }
    }
    return str;
}
