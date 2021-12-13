#include <iostream>
#include <vector>
#include <sys/types.h>  // fork
#include <unistd.h>     // fork

std::vector<std::string> splitString(std::string str, char delim) {
  std::vector<std::string> result;
  std::string::iterator first = str.begin();
  for (auto itr = str.begin(); itr != str.end(); itr++) {
    if (*itr == delim){
      result.push_back(std::string(first, itr));
      first = itr + 1;
      continue;
    }
  }
  result.push_back(std::string(first, str.end()));
  return result;
}

int main() {
  while (true) {
    std::string buf;
    std::getline(std::cin, buf);
    auto commands = splitString(buf, ';');
#ifdef DEBUG
    for (auto itr = commands.begin(); itr != commands.end(); itr++) {
      std::cout << "[+] Commands: " << *itr << std::endl;
    }
#endif
    for (auto itr = commands.begin(); itr != commands.end(); itr++) {
      pid_t pid = fork();
      if (pid == -1) exit(EXIT_FAILURE);
      else if (pid == 0) {
        // child
        system((*itr).c_str());
        exit(EXIT_SUCCESS);
      }
    }
  }
}