#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

void trim(string&);
void convertVarToVal(string&);
void cmdPrint(string&);
void cmdRepeat(string&);
void cmdVar(string&);
void parseLine(string&);

//helper functions
void trim(string &s){
    int start = 0, end = s.size() - 1;
    int prevS, prevE;
    while(start <= end && (s[start] == ' ' || s[start] == '\t')){
        ++start;
    }
    
    while(end >= start && (s[end] == ' ' || s[end] == '\t')){
        --end;
    }
    s = s.substr(start, end - start + 1);
}

unordered_map<string,string> varMap;
void convertVarToVal(string &s){
    for(auto it : varMap){
        size_t pos = 0;
        while((pos = s.find('$' + it.first, pos)) != string::npos){
            s.replace(pos, it.first.size() + 1, it.second);
            pos += it.second.size();
        }
    }
}

typedef void (*CommandFunc)(string&);

unordered_map<string, CommandFunc> commandMap = {
    {"print", cmdPrint},
	{"repeat", cmdRepeat},
    {"var", cmdVar}
};


//command functions
void cmdPrint(string& arg){
    convertVarToVal(arg);
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
    int idx = arg.find(' ');
    string varName = arg.substr(0, idx);
    idx = arg.find('=') + 1;
    string varValue = arg.substr(idx);
    trim(varValue);
    varMap[varName] = varValue;
}

void parseLine(string &s) {
    auto spacePos = s.find(' ');
    if (spacePos != string::npos) {
        string command = s.substr(0, spacePos);
        if (commandMap.count(command)) {
            string arg = s.substr(spacePos + 1);
            commandMap[command](arg);
            return;
        }
    }
    cout << "Error: no such command as " << s << '\n';
}

int main() {
    string s;
    while (true) {
        cout << "command: ";
        getline(cin, s);
        if (!s.compare("return")) {
            return 0;
        }
        parseLine(s);
    }
    return 0;
}

