//#include "code_class_helpers.hpp"
#include "part3_helpers.hpp"

Code::Code(){}

int Code::nextQuad(){
    return codeVector.size() + 1;
}

void Code::print(ofstream& f){
    for(string c : codeVector){
        f << c << endl;
    }
}

void Code::backpatch(CodeList lineList, int lineNumber){
    const string repFrom = " -1";
    string repTo = " " + to_string(lineNumber);
    for(int codeLine : lineList){
        if(!replace(codeVector[codeLine - 1], repFrom, repTo)){
            cerr << "Code::backpatch Error parsing line to backpatch:" << endl
                << "Line " << codeLine << ": " << codeVector[codeLine - 1] << endl;
            exit(1);
        }
    }
}


bool replace(string& str, const string& from, const string& to) {
    size_t start = str.find(from);
    if(start == string::npos)
        return false;
    str.replace(start, from.length(), to);
    return true;
}


CodeList::CodeList(){}

CodeList::CodeList(int code){
    this->push_back(code);
}

