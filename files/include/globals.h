#ifndef QK_GLOBALS_H
#define QK_GLOBALS_H
#define TOTAL_COMMANDS 12
#include "prototypes.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
typedef void (*CommandFunc)(std::string&);
extern bool running;
extern std::ofstream *ptr;
extern std::string fileName;
extern std::unordered_map<std::string,std::string> varMap;
extern std::unordered_map<std::string, std::string> escCharMap;
extern std::ofstream history_file;
extern const std::string helper_text[TOTAL_COMMANDS];
extern const std::unordered_set<char> disallowedNameSet;
extern std::unordered_map<std::string, CommandFunc> commandMap;
#endif
