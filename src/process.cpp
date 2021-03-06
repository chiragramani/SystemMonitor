#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : _pid(pid), _user(LinuxParser::User(pid)), _command(LinuxParser::Command(pid)) {};

// Pid
int Process::Pid() const { return _pid; }

// Cpu Utilization
float Process::CpuUtilization() const { 
  LinuxParser::ProcessCPUInfo info =  LinuxParser::CpuUtilization(_pid);
  const float totalTime = info.uTime + info.sTime + info.cutime + info.cstime;
  const float seconds = LinuxParser::UpTime() - info.startTime;
  const float utilization = (totalTime / seconds);
  return utilization;
}

// Command
string Process::Command() const { return _command; }

// RAM
string Process::Ram() { return LinuxParser::Ram(_pid); }

// User
string Process::User() const { return _user; }

// Uptime
long int Process::UpTime() const { return LinuxParser::UpTime(_pid); }

// > operator overloading
bool Process::operator>(Process const& a) const { 
 return (CpuUtilization() > a.CpuUtilization());
}