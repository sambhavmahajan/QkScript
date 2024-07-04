#include "../include/prototypes.h"
#include "../include/globals.h"
void lTrim(std::string &s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t')) {
        ++start;
    }
    s = s.substr(start);
}

void rTrim(std::string &s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == ' ' || s[end - 1] == '\t')) {
        --end;
    }
    s = s.substr(0, end);
}
void trim(std::string &s){
    lTrim(s);
    rTrim(s);
}

void formatString(std::string &s) {
    for(auto it = varMap.begin(); it != varMap.end(); ++it){
        size_t pos = 0;
        while(pos < s.size() && (pos = s.find('$' + it->first,pos)) != std::string::npos){
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
        while(pos < s.size() && (pos = s.find(it->first,pos)) != std::string::npos){
            s.replace(pos, it->first.size(), it->second);
            pos += it->second.size();
        }
    }
}
