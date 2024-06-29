#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

typedef void (*CommandFunc)(const string&);
void cmdPrint(const string& arg){
    if(arg.size() == 6) return;
    cout << arg.substr(5,arg.size()) << '\n';
}
unordered_map<string, CommandFunc> commandMap = {
    {"print", cmdPrint}
};

void parseLine(const string &s) {
    auto spacePos = s.find(' ');
    if (spacePos != string::npos) {
        string command = s.substr(0, spacePos);
        if (commandMap.count(command)) {
            cout << s.substr(spacePos + 1) << '\n';
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

