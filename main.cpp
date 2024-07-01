#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

bool running = true;

void trim(string&);
void formatString(string&);
void cmdPrint(string&);
void cmdRepeat(string&);
void cmdVar(string&);
void cmdOpen(string&);
void cmdWrite(string&);
void cmdClose(string&);
void cmdReturn(string&);
void parseLine(string&);

//helper functions
void lTrim(string &s, int& start, int end){
    while(start <= end && (s[start] == ' ' || s[start] == '\t')){
        ++start;
    }
    s = s.substr(start, end - start + 1);
    start = 0;
}
void rTrim(string &s, int start, int& end){
    while(end >= start && (s[end] == ' ' || s[end] == '\t')){
        --end;
    }
    s = s.substr(start, end - start + 1);
}
void trim(string &s){
    int start = 0, end = s.size() - 1;
    lTrim(s, start, end);
    rTrim(s, start, end);
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
            if(pos - 1 >= 0 && s[pos - 1] == '\\'){
                s.erase(pos - 1, 1);
                pos += it->first.size();
            }else{
                s.replace(pos, it->first.size() + 1, it->second);
                pos += it->second.size();
            }
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
    {"return", cmdReturn},
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
string fileName = "";
void cmdOpen(string &arg){
    trim(arg);
    if(ptr) cmdClose(arg);
    ptr = new ofstream(arg);
    if (!ptr->is_open()) {
        cout << "Error: Could not open file." << arg << '\n';
        delete ptr;
        ptr = nullptr;
        return;
    }
    fileName = arg;
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
    if(!ptr){
        cout<<"No file open.\n";
    }
    cout<<"Closing "<<fileName<<'\n';
    ptr->close();
    delete ptr;
    ptr = nullptr;
    cout<<fileName<<" closed.\n";
}
void cmdReturn(string&){
    if(ptr){
        cout<<"Closing "<<fileName<<'\n';
        ptr->close();
        delete ptr;
        cout<<fileName<<" closed.\n";
    }
    running = false;
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

void run(string &s,int pass){
    cout << "command: ";
    getline(cin, s);
    lTrim(s, pass,s.size() - 1);
    parseLine(s);
}

int main() {
    string s;
    while (running) {
        run(s,0);
    }
    return 0;
}
