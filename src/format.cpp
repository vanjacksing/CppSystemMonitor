#include <string>
#include <iomanip>
#include <iostream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::ostringstream stream;
    int hour = seconds / 3600;
    int min = (seconds - hour*3600)/60;
    int sec = seconds % 60;
    stream << std::setw(2) << std::setfill('0') << std::to_string(hour) << ":" 
     << std::setw(2) << std::setfill('0') << std::to_string(min) << ":"
     << std::setw(2) << std::setfill('0') << std::to_string(sec);
    return stream.str();
}