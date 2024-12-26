#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage

  while(true){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    
    if(input == "exit 0"){
      break;
    }// checks for exit condition 0

    std::string cmdCheck = input.substr(0,4);
    std::string restOfInput = input.substr(5);

    switch(cmdCheck){
      case "echo":
        std::cout << restOfInput << std::endl;

      case "type":
        if(restOfInput.find("type") != "npos" || restOfInput.find("echo") != "npos" || restOfInput.find("exit") != "npos"){
          std::cout << restOfInput << "is a shell builtin\n"; 
        }
        else{
          std::cout << restOfInput << ": not found\n";
        }

      default:
        std::cout << input << ": command not found\n";
    }



  };
}
