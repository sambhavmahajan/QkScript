#include "../include/globals.h"
#include "../include/prototypes.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <limits.h>
void cmdHelp(std::string&){
    size_t i = 0;
    for(const std::string &s : helper_text){
        std::cout<<std::setw(5)<<++i<<"  "<<s<<'\n';
    }
}
void cmdPrint(std::string& arg){
    formatString(arg);
    std::cout << arg <<'\n';
}
void cmdRepeat(std::string& arg){
	size_t intSize = arg.find(' ');
    if(intSize == std::string::npos){
        std::cout<<"Error: Bad syntax.\n";
        return;
    }
	int i;
    try{
        i = std::stoi(arg.substr(0,intSize));
        if(i < 0){
            std::cout << "Can not repeat negative times.\n";
            return;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Can only repeat integer times.\n";
        return;
    } catch (const std::out_of_range& e) {
        std::cout<< "Out of range: can only repeat "<< INT_MAX << " times.\n";
        return;
    }
	std::string s = arg.substr(intSize + 1);
	while(i-- > 0){
		parseLine(s);
	}
}
void cmdVar(std::string &arg){
    size_t idx = arg.find('=');
    if(idx == std::string::npos){
        std::cout<<"Error: A variable must be assigned.\n";
        return;
    }
    std::string varName = arg.substr(0, idx);
    trim(varName);
    if(varName.empty()){
        std::cout<<"Error: Variable must have a name.\n";
        return;
    }
    if(commandMap.find(varName) != commandMap.end()){
        std::cout<<"Error: Variable name can not have the same name as a command.\n";
        return;
    }
    if(std::isdigit(varName[0])){
        std::cout<<"Error: Variable name can not start with a digit.\n";
        return;
    }
    for(const char &c: varName){
        if(!(std::isalnum(c) || c == '_')){
            std::cout<<"Variable names can only contain alphanumeric characters and underscores\n";
            return;
        }
    }
    if(++idx < arg.size()){
        std::string varValue = "";
        varValue = arg.substr(idx);
        trim(varValue);
        formatString(varValue);
        varMap[varName] = varValue;
    }
    else varMap[varName] = "";
}

void cmdOpen(std::string &arg){
    trim(arg);
    if(ptr) cmdClose(arg);
    ptr = new std::ofstream(arg, std::ios::app);
    if (!ptr->is_open()) {
        std::cout << "Error: Could not open file." << arg << '\n';
        delete ptr;
        ptr = nullptr;
        return;
    }
    fileName = arg;
}
void cmdWrite(std::string &arg){
    if(!ptr || !ptr->is_open()){
        std::cout<<"Error: No file is open.\n";
        return;
    }
    formatString(arg);
    (*ptr)<<arg;
}
void cmdClose(std::string&){
    if(!ptr){
        std::cout<<"No file open.\n";
        return;
    }
    std::cout<<"Closing "<<fileName<<'\n';
    if(ptr->is_open()) ptr->close();
    delete ptr;
    ptr = nullptr;
    std::cout<<fileName<<" closed.\n";
}
void cmdReturn(std::string&){
    if(history_file.is_open()){
        history_file.close();
    }
    if(ptr){
        std::cout<<"Closing "<<fileName<<'\n';
        if(ptr->is_open()) ptr->close();
        delete ptr;
        std::cout<<fileName<<" closed.\n";
    }
    running = false;
}
void cmdHistory(std::string &arg){
    if(history_file.is_open()) history_file.close();
    std::ifstream temp("command_history");
    trim(arg);
    if(arg.compare("")){
        std::cout<<"Error: history does not takes any arguments.\n";
        return;
    }
    if(!temp.is_open()){
        std::cout<<"Error: could not open history file.\n";
        return;
    }
    std::string line;
    size_t i = 0;
    while(getline(temp, line)){
        std::cout<<std::setw(5)<<++i<<"  "<<line<<'\n';
    }
    temp.close();
    history_file.open("command_history", std::ios::app);
}
void cmdRead(std::string &arg){
    formatString(arg);
    std::ifstream file(arg);
    if (file.good()) {
        std::string line;
        while (getline(file, line)) {
            std::cout << line << '\n';
        }
        file.close();
    } else {
        std::cout << "Error: File does not exist or cannot be opened.\n";
    }
}
void cmdMkdir(std::string& arg){
    formatString(arg);
    if(!arg.compare("")){
        std::cout<<"Error: Missing <foldername>.\n";
        return;
    }
    if(std::filesystem::create_directory(arg)){
        std::cout<<"Folder created successfully.\n";
    }else std::cout<<"Error: Could not create folder or it already exits.\n";
}
