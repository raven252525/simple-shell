#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>

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
    
    std::string cmdCheck = input.substr(0,4);
    std::string restOfInput = input.substr(5);//string extraction for keywords

    if(cmdCheck == "echo"){
      std::cout << restOfInput << std::endl;
    }//echo
    else if(cmdCheck == "type"){
      if(restOfInput.find("type") == 0 || restOfInput.find("echo") == 0 || restOfInput.find("exit") == 0){
        std::cout << restOfInput << " is a shell builtin\n"; 
      }
      else{
        std::string path = getPath(restOfInput); // returns the path of the input to string
        if(path.empty()){
          std::cout << restOfInput << ": not found\n";
        }
        else{
          std::cout << restOfInput << " is " << path << std::endl;
        }//path in type
      }
    }//type
    else if(pathCheck != ""){


        const char* cstrPath = pathCheck.c_str(); //converts string to cstring char*
        char *const argv[] = {const_cast<char*>("/bin/ls"), const_cast<char*>(restOfInput.c_str()), nullptr};
        char *const envp[] = {nullptr}; // initializes an array of null terminated strings repping environment variables
        execve(cstrPath, argv, envp);
    }//program exe
    else{
      std::cout << input << ": command not found\n";
    }//invalid
  }
}
