#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <sys/wait.h> /*For waitpid,
gives error bc windows is not in POSIX compliant environment,
but in Linux system, program runs*/

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

void executeChild(std::string pathCh, std::vector<std::string> args){
  
  pid_t pid = fork(); //create parent child process

  if (pid < 0) {
      // Error occurred during fork
      perror("fork failed");
  }
  else if (pid == 0) {
    // Child process
    const char* cPath = pathCh.c_str(); //converts string to cstring char*
    char *const argv[] = {const_cast<char*>("/bin/ls"), const_cast<char*>(args[1].c_str()), nullptr};
    char *const envp[] = {nullptr};

    execve(cPath, argv, envp);
  }
  else{
    // Parent process
    int status;
    waitpid(pid, &status, 0); // Wait for the child process to complete
  }
}

int checkCharFrequency(std::string s, char toCheck){
  int count = 0;
  for(int i = 0; i < s.size(); i++){
    if(s[i] == toCheck){
      count++;
    } 
  }

  return count;
}//checks the frequency of a character in a string

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

    /*prepatory work for prorgam exe, we put it after echo, type, and exit keywords 
    bc we want to check those processes first, and we need this for the if conditional*/
    std::stringstream stream(input);
    std::string pathCheck;
    stream >> pathCheck;//get the entire potential path of the first arg
    pathCheck = getPath(pathCheck); //reinitilaize the firt arg as a path, if not, then its an empty string
    stream.str(""); stream.clear();

    std::vector<std::string> arguments;
    arguments = split_string(input, ' ');//specifically when we need to isolate args

    if(arguments[0] == "echo"){
      std::string stringLit = input.substr(5);
      if(stringLit.front() == '\'' && stringLit.back() == '\''){
        std::cout << stringLit.substr(1, stringLit.size() - 2) << std::endl;
      }
      else{
        std::stringstream S{stringLit.data()};
        std::string temp;
        int i = 0;
        while(S >> temp){
          std::string temp2 = ' ' + temp;
          std::cout << (i++ ? temp2 : temp); // this essentially will return a space followed by the string except for the first case
        }
        std::cout << std::endl;
      }

    }//echo
    else if(arguments[0] == "cat"){
      std::string files = input.substr(4);
      if(files.front() == '\'' && files.back() == '\''){
        files = files.substr(1, files.length() - 2);
        
        std::ifstream fl(files);
        if(!fl.is_open()){
          std::cout << "Cannot open file\n";
          break;
        }

        std::string line;
        while (std::getline(fl, line)) {
          std::cout << line << std::endl;
        }

        fl.close();
      }
      else{
        std::cout << "not a valid input\n";
      }
    }
    else if(arguments[0] == "type"){
      if(arguments[1] == "type" || arguments[1] == "echo" || arguments[1] == "exit" || arguments[1] == "pwd" || arguments[1] == "cd"){
        std::cout << arguments[1] << " is a shell builtin\n"; 
      }
      else{
        std::string path = getPath(input.substr(5)); // returns the path of the input to string
        if(path.empty()){
          std::cout << input.substr(5) << ": not found\n";
        }
        else{
          std::cout<< input.substr(5) << " is " << path << std::endl;
        }//path in type
      }
    }//type
    else if(arguments[0] == "pwd"){
      std::string pWD = std::filesystem::current_path();
      std::cout << pWD << std::endl;
    }//pwd
    else if(arguments[0] == "cd"){
        if(arguments[1] == "~"){
          std::string homeEnv = std::getenv("HOME");
          std::filesystem::current_path(homeEnv);
        }
        else if(std::filesystem::exists(arguments[1])){
          std::filesystem::current_path(arguments[1]);
        }
        else{
          std::cout << arguments[1] << ": No such file or directory\n"; 
        }
    }//cd to absolute path, ~(homedir), AND relative path surprisingly! Live laugh love the standard library
    else if(pathCheck != ""){
      executeChild(pathCheck, arguments); 
    }//program exe
    else{
      std::cout << input << ": command not found\n";
    }//invalid
  }
}
