#include "processor.h"
#include "linux_parser.h"
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long active = LinuxParser::ActiveJiffies();
  long idle = LinuxParser::IdleJiffies();
  long total = active + idle;
  long lastTotal = lastActive + lastIdle;
  long totald = total - lastTotal;
  long idled = idle - lastIdle;
  lastActive = active;
  lastIdle = idle;
  return float(totald - idled)/float(totald); 
}