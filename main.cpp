#include <iostream>
#include <vector>
#include <array>
#include <sys/types.h>  // fork
#include <unistd.h>     // fork

#define READ  (0)
#define WRITE (1)

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
    for (auto cmd: commands) {
      std::cout << "[+] Commands: " << *itr << std::endl;
    }
#endif
    for (auto piped_commands: commands) {
      auto piped_commands_vec = splitString(piped_commands, '|');

      // パイプを準備
      std::vector<int*> pipe_descriptors;
      for (int i = 0; i < piped_commands_vec.size(); i++) {
        int* ptr = (int*)malloc(sizeof(int) * 2);
        pipe_descriptors.push_back(ptr);
        if (pipe(ptr) < 0) {
          exit(EXIT_FAILURE);
          close(ptr[READ]);
          close(ptr[WRITE]);
          std::cerr << "Failed to open pipe" << std::endl;
          break;
        }
      }

      const int num_piped_cmds = piped_commands_vec.size();
      int idx = 0; // pipeのインデックス
      for (auto cmd: piped_commands_vec) {
        pid_t pid_inner = fork();
        if (pid_inner < 0) exit(EXIT_FAILURE);
        else if (pid_inner == 0) {
          // child
#ifdef DEBUG
          std::cerr << "[+] Start process " << idx << cmd << std::endl;
#endif
          const bool is_last_cmd = (idx == num_piped_cmds - 1);
          const bool is_first_cmd = (idx == 0);
          if (is_first_cmd && !is_last_cmd){
            // 先頭のコマンド
            // 一つ後ろに書く
            close(pipe_descriptors.at(0)[READ]);
            dup2(pipe_descriptors.at(0)[WRITE], 1);
            close(pipe_descriptors.at(0)[WRITE]);
          } else if (is_last_cmd && !is_first_cmd) {
            // 最後尾のコマンド
            // 一つ前から読み込む
            close(pipe_descriptors.at(idx - 1)[WRITE]);
            dup2(pipe_descriptors.at(idx - 1)[READ], 0);
            close(pipe_descriptors.at(idx - 1)[READ]);
          } else if (!is_last_cmd && !is_first_cmd){
            // 途中のコマンド
            // 一つ前から読み込んで
            close(pipe_descriptors.at(idx - 1)[WRITE]);
            dup2(pipe_descriptors.at(idx - 1)[READ], 0);
            close(pipe_descriptors.at(idx - 1)[READ]);

            // 一つ後ろに書く
            close(pipe_descriptors.at(idx)[READ]);
            dup2(pipe_descriptors.at(idx)[WRITE], 1);
            close(pipe_descriptors.at(idx)[WRITE]);
          }
          sleep(0.1); // 後ろのスレッドのパイプ設定が終わるまで待つ
          // 実行
          system(cmd.c_str());
          exit(EXIT_SUCCESS);
        } else {
          // parent
          idx += 1;
        }
      }
    }
  }
}
