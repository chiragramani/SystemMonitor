#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  Processor();

 private:
  float _utilization;
  float _previousTotalCPUTime;
  float _previousIdleCPUTime;
};

#endif