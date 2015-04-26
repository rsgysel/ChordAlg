#ifndef INCLUDE_CHORDALG_STRING_H_
#define INCLUDE_CHORDALG_STRING_H_

#include <cstring>
#include <string>
#include <vector>
#include <utility>

namespace chordalg {

typedef std::vector< std::string > StringTokens;
typedef std::pair< std::string, std::string > StringPair;

// True iff str is a natural number
bool IsNum(std::string str);

// Splits string and changes chars to lowercase
StringTokens Split(std::string str, std::string delim);

}  // namespace ChordAlg

#endif  // INCLUDE_CHORDALG_STRING_H_

