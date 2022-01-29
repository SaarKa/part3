//#include "code_class_helpers.hpp"
#include "part3_helpers.hpp"

Code::Code(){}

int Code::nextQuad(){
    return codeVector.size()+1;
}

bool replace(string& str, const string& from, const string& to) {
    size_t begin = str.find(from);
    if(begin == string::npos)
        return false;
    str.replace(begin, from.length(), to);
    return true;
}


CodeList::CodeList(){}

CodeList::CodeList(int line){
    this->push_back(line);
}

/*
template<typename T, typename... Args>
string Code::typeString(T t, Args... args) {
    return typeString(t)+string(" ")+typeString(args...);
}

string Code::typeString(string str) {return str;}
string Code::typeString(int num) {return to_string(num);}

template<typename... Args>
int Code::emit(Args... args){
    string code = typeString(args...);
    codeVector.push_back(code);
    return codeVector.size();
}
*/




void Code::print(ofstream& f){
    for(string codeLine : codeVector){
        f << codeLine << endl;
    }
}

void Code::backpatch(CodeList codeList, int lineNumber){
    const string repFrom = " -1";
    string repTo = " "+to_string(lineNumber);
    for(int line : codeList){
        if(!replace(codeVector[line - 1], repFrom, repTo)){
            cerr << "Code::failed to backpatch line:" << endl
                << "Line " << line << ": " << codeVector[line - 1] << endl;
            exit(1);
        }
    }
}
