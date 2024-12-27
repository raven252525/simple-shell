#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <vector>

std::string getPath(std::string command){
  std::string envPath = std::getenv("PATH");

  std::stringstream ss(envPath); // add path environment as a string stream obj
  std::string path;// where to place each of the directories separated by the : in path before adding to return str

  while(!ss.eof()){
    std::getline(ss, path, ':');
    std::string absPath = path + '/' + command;

    if(std::filesystem::exists(absPath)){
      return absPath;
    }
  }

  return "";//returns an empty string if there is no path that exists
}

std::vector<std::string> split_string(const std::string &s, char delim){
    std::stringstream SS(s);
    std::vector<std::string> return_vect;

    std::string token;

    while(getline(SS, token, delim)){
      return_vect.push_back(token);
    }
    return return_vect;
}

void executeChild(std::string cPath, std::vector<std::string> args){
  const char* cPath = pathCh.c_str(); //converts string to cstring char*
  char *const argv[] = {const_cast<char*>("/bin/ls"), const_cast<char*>(args[1].c_str()), nullptr};
  char *const envp[] = {nullptr}; // initializes an array of null terminated strings repping environment variables
  execve(pathCh, argv, envp);
}

int main(){
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    
    if(input == "exit 0"){
      break;
    }// checks for exit condition 0

    //prepatory work for prorgam exe, we put it after echo, type, and exit keywords bc we want to check those processes first, and we need this for the if conditional
    std::stringstream stream(input);
    std::string pathCheck;
    stream >> pathCheck;//get the entire potential path of the first arg
    pathCheck = getPath(pathCheck); //reinitilaize the firt arg as a path, if not, then its an empty string
    
    std::vector<std::string> arguments;
    arguments = split_string(input, ' ');//specifically when we need to isolate args

    std::string restOfInput = input.substr(5);//string extraction for keywords, and for echo and type ease of use

    if(arguments[0] == "echo"){
      std::cout << restOfInput << std::endl;
    }//echo
    else if(arguments[0] == "type"){
      if(arguments[1] == "type" || arguments[1] == "echo" || arguments[1] == "exit"){
        std::cout << arguments[1] << " is a shell builtin\n"; 
      }
      else{
        std::string path = getPath(restOfInput); // returns the path of the input to string
        if(path.empty()){
          std::cout << restOfInput << ": not found\n";
        }
        else{
          std::cout<< restOfInput << " is " << path << std::endl;
        }//path in type
      }
    }//type
    else if(pathCheck != ""){
      executeChild(pathCheck, arguments);        
    }//program exe
    else{
      std::cout << input << ": command not found\n";
    }//invalid
  }
}
