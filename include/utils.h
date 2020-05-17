#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace Utils {
  vector<long> convertToLong(vector<string> stringValues);
  string AddZeros(const unsigned int requiredLength, const int value);
};


#endif