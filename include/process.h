#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime() const;
  bool operator>(Process const& a) const;
  Process(int pid);

 private:
 int _pid;
 string _user;
 string _command;
};

#endif