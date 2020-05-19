#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//TODO: Process constructor
Process::Process(int pid) {
  pid_ = pid;
  cpuUtil_ = Process::CpuUtilization(pid);
  upTime_ = LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return cpuUtil_;
}

float Process::CpuUtilization(int pid) {
  long procJiffies = LinuxParser::ActiveJiffies(pid);
  long total = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  float cpuUtil = float(procJiffies)/float(total);
  return cpuUtil;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
  string ramString = LinuxParser::Ram(pid_);
  long ramKB;
  try {
    ramKB = std::stol(ramString);
  } catch(const std::invalid_argument) {
    ramKB = 0;
  }
  long ramMB = ramKB/1024;
  return std::to_string(ramMB);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return upTime_;  
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return cpuUtil_ < a.cpuUtil_; 
  //return upTime_ < a.upTime_;
}