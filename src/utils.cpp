#include "utils.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

vector<long> Utils::convertToLong(vector<string> stringValues) {
  vector<long> longValues;
  for(string s: stringValues) {
   try {
     longValues.push_back(std::stol(s));
   } catch (...) {
     longValues.push_back((long)0);
   }
  }
  return longValues;
}

string Utils::AddZeros(const unsigned int requiredLength, const int value) {
 string stringValue = std::to_string(value);
 while(stringValue.length() < requiredLength) {
  stringValue = "0" + stringValue;
 }
  return stringValue;
}
