#include <iostream>
#include "../include/prototypes.h"
#include "../include/globals.h"

void parseLine(std::string &s) {
    if(history_file.is_open()){
        history_file<<s<<'\n';
    }
    size_t spacePos = s.find(' ');
    std::string command = s.substr(0, spacePos);
    if (spacePos == std::string::npos) spacePos = s.size() - 1;
    if (commandMap.count(command)) {
        std::string arg;
        if(spacePos != s.size() - 1) arg = s.substr(spacePos + 1);
        commandMap[command](arg);
        return;
    }
    std::cout << "Error: no such command as " << s << '\n';
}

void run(std::string &s){
    std::cout << "command: ";
    std::getline(std::cin, s);
    lTrim(s);
    parseLine(s);
}

int main() {
    std::string s;
    while (running) {
        run(s);
    }
    return 0;
}

