#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long lastActive{0};
  long lastIdle{0};
};

#endif