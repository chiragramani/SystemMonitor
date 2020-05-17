#include <string>

#include "format.h"
#include "utils.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  if (seconds < 0) { return "00::00::00"; }
  const int hours = seconds / 3600;
  const int minutes = (seconds / 60) % 60;
  const int sec = seconds % 60;
  
  const string hoursPadded = Utils::AddZeros(2, hours);
  const string minutesPadded = Utils::AddZeros(2, minutes);
  const string secondsPadded = Utils::AddZeros(2, sec);
  
  string elapsedTime = hoursPadded + ":" + minutesPadded + ":" + secondsPadded;
  return elapsedTime; 
}