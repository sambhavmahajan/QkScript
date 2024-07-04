#include "../include/globals.h"
bool running = true;
std::ofstream *ptr = nullptr;
std::string fileName = "";
std::unordered_map<std::string,std::string> varMap;
std::unordered_map<std::string, std::string> escCharMap = 
{
    {"\\n", "\n"},
    {"\\t", "\t"},
    {"\\r", "\r"},
    {"\\\"", "\""},
    {"\\\\", "\\"}
};
std::ofstream history_file("command_history", std::ios::app);
const std::string helper_text[TOTAL_COMMANDS] = {
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
const std::unordered_set<char> disallowedNameSet = {
        ' ', '!', '#', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/',
        ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_',
        '`', '{', '|', '}', '~'
    };
std::unordered_map<std::string, CommandFunc> commandMap = {
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
