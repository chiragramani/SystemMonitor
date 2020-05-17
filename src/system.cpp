#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// Constructor
System::System() {
 kernel_ = LinuxParser::Kernel();
 operatingSystem_= LinuxParser::OperatingSystem();
} 

// Processes
vector<Process>& System::Processes() { 
  processes_.clear();
  const vector<int> pids = LinuxParser::Pids();
  for (int pid: pids) {
   processes_.push_back(Process(pid));
  }
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

// Kernel
std::string System::Kernel() { return kernel_; }

// MemoryUtilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// OperatingSystem
std::string System::OperatingSystem() { return operatingSystem_; }

// RunningProcesses
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TotalProcesses
int System::TotalProcesses() { return LinuxParser::TotalProcesses() ; }

// UpTime
long int System::UpTime() { return LinuxParser::UpTime(); }