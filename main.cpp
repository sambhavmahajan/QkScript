#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <limits.h>
using namespace std;

void lTrim(string&);
void rTrim(string&);
void trim(string&);
void formatString(string&);
void cmdPrint(string&);
void cmdRepeat(string&);
void cmdVar(string&);
void cmdOpen(string&);
void cmdWrite(string&);
void cmdClose(string&);
void cmdReturn(string&);
void cmdHistory(string&);
void cmdRead(string&);
void cmdHelp(string&);
void cmdMkdir(string&);
void parseLine(string&);

//global variables
bool running = true;
ofstream *ptr = nullptr;
string fileName = "";
unordered_map<string,string> varMap;
unordered_map<string, string> escCharMap = 
{
    {"\\n", "\n"},
    {"\\t", "\t"},
    {"\\r", "\r"},
    {"\\\"", "\""},
    {"\\\\", "\\"}
};
ofstream history_file("command_history", ios::app);
const string helper_text[] = {
        "print <message>: Prints a message.",
        "repeat <n> <command>: Repeats command(s) n times.",
        "var <name> = <value>: Declares or updates a variable.",
        "print $<varname>: Fetches the value of a variable.",
        "open <filename>: Opens or creates a file for writing.",
        "write <content>: Writes content to the opened file.",
        "close: Closes the currently opened file.",
        "return: Closes the terminal.",
        "history: Gives the command history.",
        "read <filename>: Reads content of a file.",
        "help: Gives list of all commands.",
        "mkdir <foldername>: Creates a new folder."
    };
const unordered_set<char> disallowedNameSet = {
        ' ', '!', '#', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/',
        ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_',
        '`', '{', '|', '}', '~'
    };
//helper functions
void lTrim(string &s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t')) {
        ++start;
    }
    s = s.substr(start);
}

void rTrim(string &s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == ' ' || s[end - 1] == '\t')) {
        --end;
    }
    s = s.substr(0, end);
}
void trim(string &s){
    lTrim(s);
    rTrim(s);
}

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
    {"write",cmdWrite},
    {"history", cmdHistory},
    {"read", cmdRead},
    {"help", cmdHelp},
    {"mkdir", cmdMkdir}
};


//command functions
void cmdHelp(string&){
    size_t i = 0;
    for(const string &s : helper_text){
        cout<<setw(5)<<++i<<"  "<<s<<'\n';
    }
}
void cmdPrint(string& arg){
    formatString(arg);
    cout << arg <<'\n';
}
void cmdRepeat(string& arg){
	size_t intSize = arg.find(' ');
    if(intSize == string::npos){
        cout<<"Error: Bad syntax.\n";
        return;
    }
	int i;
    try{
        i = stoi(arg.substr(0,intSize));
        if(i < 0){
            cout << "Can not repeat negative times.\n";
            return;
        }
    } catch (const invalid_argument& e) {
        cout << "Can only repeat integer times.\n";
        return;
    } catch (const out_of_range& e) {
        cout<< "Out of range: can only repeat "<< INT_MAX << " times.\n";
        return;
    }
	string s = arg.substr(intSize + 1);
	while(i-- > 0){
		parseLine(s);
	}
}
void cmdVar(string &arg){
    size_t idx = arg.find('=');
    if(idx == string::npos){
        cout<<"Error: A variable must be assigned.\n";
        return;
    }
    string varName = arg.substr(0, idx);
    trim(varName);
    if(varName.empty()){
        cout<<"Error: Variable must have a name.\n";
        return;
    }
    if(commandMap.find(varName) != commandMap.end()){
        cout<<"Error: Variable name can not have the same name as a command.\n";
        return;
    }
    if(isdigit(varName[0])){
        cout<<"Error: Variable name can not start with a digit.\n";
        return;
    }
    for(const char &c: varName){
        if(!(isalnum(c) || c == '_')){
            cout<<"Variable names can only contain alphanumeric characters and underscores\n";
            return;
        }
    }
    if(++idx < arg.size()){
        string varValue = "";
        varValue = arg.substr(idx);
        trim(varValue);
        formatString(varValue);
        varMap[varName] = varValue;
    }
    else varMap[varName] = "";
}

void cmdOpen(string &arg){
    trim(arg);
    if(ptr) cmdClose(arg);
    ptr = new ofstream(arg, ios::app);
    if (!ptr->is_open()) {
        cout << "Error: Could not open file." << arg << '\n';
        delete ptr;
        ptr = nullptr;
        return;
    }
    fileName = arg;
}
void cmdWrite(string &arg){
    if(!ptr || !ptr->is_open()){
        cout<<"Error: No file is open.\n";
        return;
    }
    formatString(arg);
    (*ptr)<<arg;
}
void cmdClose(string&){
    if(!ptr){
        cout<<"No file open.\n";
        return;
    }
    cout<<"Closing "<<fileName<<'\n';
    if(ptr->is_open()) ptr->close();
    delete ptr;
    ptr = nullptr;
    cout<<fileName<<" closed.\n";
}
void cmdReturn(string&){
    if(history_file.is_open()){
        history_file.close();
    }
    if(ptr){
        cout<<"Closing "<<fileName<<'\n';
        if(ptr->is_open()) ptr->close();
        delete ptr;
        cout<<fileName<<" closed.\n";
    }
    running = false;
}
void cmdHistory(string &arg){
    if(history_file.is_open()) history_file.close();
    ifstream temp("command_history");
    trim(arg);
    if(arg.compare("")){
        cout<<"Error: history does not takes any arguments.\n";
        return;
    }
    if(!temp.is_open()){
        cout<<"Error: could not open history file.\n";
        return;
    }
    string line;
    size_t i = 0;
    while(getline(temp, line)){
        cout<<setw(5)<<++i<<"  "<<line<<'\n';
    }
    temp.close();
    history_file.open("command_history", ios::app);
}
void cmdRead(string &arg){
    formatString(arg);
    ifstream file(arg);
    if (file.good()) {
        string line;
        while (getline(file, line)) {
            cout << line << '\n';
        }
        file.close();
    } else {
        cout << "Error: File does not exist or cannot be opened.\n";
    }
}
void cmdMkdir(string& arg){
    formatString(arg);
    if(!arg.compare("")){
        cout<<"Error: Missing <foldername>.\n";
        return;
    }
    if(filesystem::create_directory(arg)){
        cout<<"Folder created successfully.\n";
    }else cout<<"Error: Could not create folder or it already exits.\n";
}
void parseLine(string &s) {
    if(history_file.is_open()){
        history_file<<s<<'\n';
    }
    size_t spacePos = s.find(' ');
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

void run(string &s){
    cout << "command: ";
    getline(cin, s);
    lTrim(s);
    parseLine(s);
}

int main() {
    string s;
    while (running) {
        run(s);
    }
    return 0;
}
