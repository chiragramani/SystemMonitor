#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include "utils.h"
using std::vector;

// Constructor
Processor::Processor() : _previousTotalCPUTime(0.0), _previousIdleCPUTime(0.0) {}

// Returns the processor's utilization
float Processor::Utilization() {
  vector<long> aggregatedValues = Utils::convertToLong(LinuxParser::CpuUtilization());
  if (aggregatedValues.size() == 10) {
    // Not considering guest and guest_nice times since they are already included in the user and nice times.
    const int userTime = aggregatedValues[LinuxParser::CPUStates::kUser_];
    const int niceTime = aggregatedValues[LinuxParser::CPUStates::kNice_];
    const int systemTime = aggregatedValues[LinuxParser::CPUStates::kSystem_];
    const int idleTime = aggregatedValues[LinuxParser::CPUStates::kIdle_];
    const int iowaitTime = aggregatedValues[LinuxParser::CPUStates::kIOwait_];
    const int irqTime = aggregatedValues[LinuxParser::CPUStates::kIRQ_];
    const int softirqTime = aggregatedValues[LinuxParser::CPUStates::kSoftIRQ_];
    const int stealTime = aggregatedValues[LinuxParser::CPUStates::kSteal_];
    
    /// Total time.
    const float totalTime = userTime + niceTime + systemTime + idleTime + iowaitTime + irqTime + softirqTime + stealTime;
    
    // Idle time.
    const float effectiveIdleTime = idleTime + iowaitTime;
    
    const float deltaTotal = totalTime - _previousTotalCPUTime;
    const float deltaIdle = effectiveIdleTime - _previousIdleCPUTime;
    const float deltaUsage = (deltaTotal - deltaIdle) / deltaTotal;
    
    _previousTotalCPUTime = totalTime;
    _previousIdleCPUTime = effectiveIdleTime;
    
    return deltaUsage;
  } else {
     return 0.0;
  }
}