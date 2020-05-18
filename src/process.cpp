#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//TODO: Process constructor
Process::Process(int pid) {
  pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long procJiffies = LinuxParser::ActiveJiffies(pid_);
  long total = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  cpuUtil_ = float(procJiffies)/float(total);
  return cpuUtil_;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
  char formattedRam[20];
  string ramString = LinuxParser::Ram(pid_);
  float ramMB = truncf(100 * std::stof(ramString)/1024)/100;
  sprintf(formattedRam, "%.2f", ramMB);
  return formattedRam;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return cpuUtil_ < a.cpuUtil_; 
}