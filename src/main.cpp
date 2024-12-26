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

    std::string echoCheck = input.substr(0,4);
    if(echoCheck == "echo"){
      int length = input.length() - echoCheck.length(); 
      std::string echoPrint = input.substr(6, length);
      std::cout << echoPrint << std::endl;
    }

    
    std::cout << input << ": command not found\n";
  };
}
