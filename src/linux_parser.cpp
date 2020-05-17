#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os,version,kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// References:
// 1. https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
// 2. https://support.site24x7.com/portal/kb/articles/how-is-memory-utilization-calculated-for-a-linux-server-monitor
float LinuxParser::MemoryUtilization() { 
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float totalMemory = 0, freeMemory = 0, buffersMemory = 0, cachedMemory = 0, sReclaimableMemory = 0, shmemory = 0;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
      if (key == "MemTotal:") {
          totalMemory = std::stof(value);
      } else if(key == "MemFree:") {
          freeMemory = std::stof(value);
      } else if(key == "Buffers:") {
          buffersMemory = std::stof(value);
      }  else if(key == "Cached:") {
          cachedMemory = std::stof(value);
      } else if(key == "SReclaimable:") {
          sReclaimableMemory = std::stof(value);
      } else if(key == "Shmem:") {
          shmemory = std::stof(value);
      }  
    }
    const float effectiveCachedMemory = cachedMemory + sReclaimableMemory - shmemory;
    const float effectiveFreeMemory = freeMemory + effectiveCachedMemory + buffersMemory;
    const float effectiveUtilisation = (totalMemory - effectiveFreeMemory) / totalMemory;
    return effectiveUtilisation;
  }
  return 0.0; 
}

// Returning the uptime
long LinuxParser::UpTime() { 
  string line, upTimeString;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTimeString;
    return std::stol(upTimeString);
  }
  return (long)0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<string> aggregatedValues;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string firstToken;;
    linestream >> firstToken;
        if (firstToken == "cpu") { 
          string aggregatedValue;
          while(linestream >> aggregatedValue) {
           aggregatedValues.push_back(aggregatedValue);
          }
        return aggregatedValues;
        }
  }
  return {};
}

// Reads and returns total processes
int LinuxParser::TotalProcesses() { 
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
      if (key == "processes") {
          return std::stoi(value);
        }
    }
  }
  return 0;
 }

// Reads and returns running processes
int LinuxParser::RunningProcesses() { 
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
      if (key == "procs_running") {
          return std::stoi(value);
        }
    }
  }
  return 0;
 }

// Returns the command
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return string();
 }

// Returns the RAM
string LinuxParser::Ram(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
      if (key == "VmSize:") {
        const long ram_in_kb = std::stol(value);
        const long ram_in_mb = ram_in_kb / 1024;
        return std::to_string(ram_in_mb);
        }
    }
  }
  return string();
}

// Returns the process's UID
string LinuxParser::Uid(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
        }
    }
  }
  return string();
}

// Returns the user associated with the PID
string LinuxParser::User(int pid) { 
  const string userId = LinuxParser::Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string userName, x, uid;
      linestream >> userName >> x >> uid;
      if (userId == uid) {
        return userName;
      }
    }
  }
  return string();

}

// Returns the processor's uptime
long LinuxParser::UpTime(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    int counter = 1;
    string value;
    std::istringstream linestream(line);
    while (counter <= LinuxParser::ProcessStates::kStartTime) {
      counter += 1;
      linestream >> value;
    }
    try {
    const long seconds = std::stol(value) / sysconf(_SC_CLK_TCK);
    return seconds;
    } catch (...) {
          return 0;
    }
  }
  return 0;
}

// Returns the ProcessCPUInfo
LinuxParser::ProcessCPUInfo LinuxParser::CpuUtilization(int pid) {
  LinuxParser::ProcessCPUInfo cpuInfo;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    string value;
    std::istringstream linestream(line);
    vector<string> intermediateData = {};
    while (linestream >> value) {
      intermediateData.push_back(value);
    }
    
    const string uTime = intermediateData[LinuxParser::ProcessStates::uTime - 1];
    const string sTime = intermediateData[LinuxParser::ProcessStates::sTime - 1];
    const string cutime = intermediateData[LinuxParser::ProcessStates::cutime - 1];
    const string cstime = intermediateData[LinuxParser::ProcessStates::cstime - 1];
    const string startTime = intermediateData[LinuxParser::ProcessStates::kStartTime - 1];
    
    cpuInfo.uTime = std::stol(uTime) / sysconf(_SC_CLK_TCK);
    cpuInfo.sTime = std::stol(sTime) / sysconf(_SC_CLK_TCK);
    cpuInfo.cutime = std::stol(cutime) / sysconf(_SC_CLK_TCK);
    cpuInfo.cstime = std::stol(cstime) / sysconf(_SC_CLK_TCK);
    cpuInfo.startTime = std::stol(startTime) / sysconf(_SC_CLK_TCK);
    return cpuInfo;
  }
   return cpuInfo;
}