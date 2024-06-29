#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

void parseLine(const string &s);

typedef void (*CommandFunc)(const string&);
void cmdPrint(const string& arg){
    cout << arg << '\n';
}
void cmdRepeat(const string& arg){
	int intSize = arg.find(' ');
	int i = stoi(arg.substr(0,intSize));
	const string s = arg.substr(intSize + 1);
	while(i-- > 0){
		parseLine(s);
	}
}
unordered_map<string, CommandFunc> commandMap = {
    {"print", cmdPrint},
	{"repeat", cmdRepeat}
};

void parseLine(const string &s) {
    auto spacePos = s.find(' ');
    if (spacePos != string::npos) {
        string command = s.substr(0, spacePos);
        if (commandMap.count(command)) {
            commandMap[command](s.substr(spacePos + 1));
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

