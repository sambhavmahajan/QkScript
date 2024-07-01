#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

void trim(string&);
void formatString(string&);
void cmdPrint(string&);
void cmdRepeat(string&);
void cmdVar(string&);
void cmdOpen(string&);
void cmdWrite(string&);
void cmdClose(string&);
void parseLine(string&);

//helper functions
void trim(string &s){
    int start = 0, end = s.size() - 1;
    while(start <= end && (s[start] == ' ' || s[start] == '\t')){
        ++start;
    }
    
    while(end >= start && (s[end] == ' ' || s[end] == '\t')){
        --end;
    }
    s = s.substr(start, end - start + 1);
}

unordered_map<string,string> varMap;
unordered_map<string, string> escCharMap = 
{
    {"\\n", "\n"},
    {"\\t", "\t"},
    {"\\r", "\r"},
    {"\\\"", "\""},
    {"\\\\", "\\"}
};

void formatString(string &s) {
    for(auto it = varMap.begin(); it != varMap.end(); ++it){
        size_t pos = 0;
        while(pos < s.size() && (pos = s.find('$' + it->first,pos)) != string::npos){
            s.replace(pos, it->first.size() + 1, it->second);
            pos += it->second.size();
        }
    }
    for(auto it = escCharMap.begin(); it != escCharMap.end(); ++it){
        size_t pos = 0;
        while(pos < s.size() && (pos = s.find(it->first,pos)) != string::npos){
            s.replace(pos, it->first.size(), it->second);
            pos += it->second.size();
        }
    }
}

typedef void (*CommandFunc)(string&);

unordered_map<string, CommandFunc> commandMap = {
    {"print", cmdPrint},
	{"repeat", cmdRepeat},
    {"var", cmdVar},
    {"open", cmdOpen},
    {"close", cmdClose},
    {"write",cmdWrite}
};


//command functions
void cmdPrint(string& arg){
    formatString(arg);
    cout << arg << '\n';
}
void cmdRepeat(string& arg){
	int intSize = arg.find(' ');
	int i = stoi(arg.substr(0,intSize));
	string s = arg.substr(intSize + 1);
	while(i-- > 0){
		parseLine(s);
	}
}
void cmdVar(string &arg){
    int idx = arg.find('=');
    string varName = arg.substr(0, idx);
    trim(varName);
    idx = arg.find('=') + 1;
    string varValue = arg.substr(idx);
    trim(varValue);
    formatString(varValue);
    varMap[varName] = varValue;
}

ofstream *ptr = nullptr;
void cmdOpen(string &arg){
    trim(arg);
    if(ptr) cmdClose(arg);
    ptr = new ofstream(arg);
    if (!ptr->is_open()) {
        cout << "Error: Could not open file." << arg << '\n';
        delete ptr;
        ptr = nullptr;
    }
}
void cmdWrite(string &arg){
    if(!ptr){
        cout<<"Error: No file is open.\n";
        return;
    }
    formatString(arg);
    (*ptr)<<arg;
}
void cmdClose(string&){
    if(!ptr) return;
    ptr->close();
    delete ptr;
    ptr = nullptr;
}
void parseLine(string &s) {
    auto spacePos = s.find(' ');
    string command = s.substr(0, spacePos);
    if (spacePos == string::npos) spacePos = s.size() - 1;
    if (commandMap.count(command)) {
        string arg;
        if(spacePos != s.size() - 1) arg = s.substr(spacePos + 1);
        commandMap[command](arg);
        return;
    }
    cout << "Error: no such command as " << s << '\n';
}

int main() {
    string s;
    while (true) {
        cout << "command: ";
        getline(cin, s);
        if (!s.compare("return")) {
            if(ptr){
                cout<<"Error: A file is open, can not exit.\n";
            }else return 0;
        }
        else parseLine(s);
    }
    return 0;
}

