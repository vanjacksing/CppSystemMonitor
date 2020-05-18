#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    char timeStr[8];
    int h = seconds / 3600;
    int m = (seconds - h*3600)/60;
    int s = seconds % 60;
    sprintf(timeStr, "%02d:%02d:%02d", h, m, s);
    return timeStr; 
}