#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <errno.h>

int main() {
  int write_parent__read_child[2];
  int write_child__read_parent[2];
  pid_t cpid;
  char buf;

  if (pipe(write_parent__read_child) == -1) {
    std::cout << "error" << std::endl << std::flush;
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  if (pipe(write_child__read_parent) == -1) {
    std::cout << "error" << std::endl << std::flush;
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cpid = fork();
  if(cpid == 0) {
    close(write_parent__read_child[1]);
    close(write_child__read_parent[0]);
    int in = write_parent__read_child[0];
    int out = write_child__read_parent[1];
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
    if(execlp("python3.9", "python3.9", "main.py", (void*)NULL) == -1) {
      std::cout << errno << std::endl;
    }
  } else {
    const char* data = "This is it\n";
    char fname[100];
    close(write_parent__read_child[0]);
    close(write_child__read_parent[1]);
    int out = write_parent__read_child[1];
    int in = write_child__read_parent[0];
    read(in, fname, 100);
    write(out, data, strlen(data));
    std::cout << "reading" << std::endl << std::flush;
    read(in, fname, 100);
    std::string s(fname);
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    std::cout << "done reading" << std::endl << std::flush;
    std::stringstream ss;
    ss << "mplayer " << "/home/kuberlog/daemon++/audio/" << s << ".wav";
    auto str = ss.str();
    std::cout << str << std::endl;
    system(str.c_str());
  }
  std::cout << "done" << std::endl;
}
