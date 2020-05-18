#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::map<string, float> memUsageMap{};
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float memUsage;
  while(std::getline(stream, line)) {
    std::string key, value;
    std::istringstream linestream(line);
    linestream >> key >> value;
    key.erase(std::remove(key.begin(), key.end(), ':'), key.end());
    memUsageMap[key] = std::stof(value);
  }
  memUsage = (memUsageMap["MemTotal"] - memUsageMap["MemFree"])/memUsageMap["MemTotal"];
  return memUsage; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime, idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> uptime >> idletime;
  return std::stol(uptime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<long> jiffies = LinuxParser::CpuUtilization();
  long sum{0};
  for (long f: jiffies) {
  	sum+=f;
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// Calculation algorithm is taken from here: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> tokens{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  while (std::getline(linestream, token, ' ')) {
    tokens.push_back(token);
  }
  // Return utime + stime + child processes
  return std::stol(tokens[13]) + std::stol(tokens[14]) + std::stol(tokens[15]) + std::stol(tokens[16]); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<long> jiffies = LinuxParser::CpuUtilization();
  return jiffies[0] + jiffies[1] + jiffies[2] + jiffies[5] + jiffies[6] + jiffies[7];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<long> jiffies = LinuxParser::CpuUtilization();
  return jiffies[3] + jiffies[4]; 
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  vector<long> jiffies{};
  string line, cpuid, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> cpuid;
  while (linestream >> value) {
  	jiffies.push_back(std::stol(value));
  }
  return jiffies; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "processes") {
    	linestream >> value;
        break;
    }
  }
  return std::stoi(value); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "procs_running") {
    	linestream >> value;
        return std::stoi(value);
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::getline(stream, line);
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "VmSize:") {
    	linestream >> value;
        return value;
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "Uid:") {
    	linestream >> value;
        break;
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, value;
  std::ifstream stream(kPasswordPath);
  while(std::getline(stream, line)) {
    vector<string> values{};
    std::istringstream linestream(line);
    while (std::getline(linestream, value, ':')) {
      values.push_back(value);
    }
    if (values[2] == uid) {
      return values[0];
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, token;
  vector<string> tokens{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  while (std::getline(linestream, token, ' ')) {
    tokens.push_back(token);
  }
  return std::stol(tokens[21])/sysconf(_SC_CLK_TCK); 
}