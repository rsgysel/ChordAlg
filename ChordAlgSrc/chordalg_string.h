/*
 * chordalg_string.h - string utility functions
 */

#ifndef CHORDALGSRC_CHORDALG_STRING_H__
#define CHORDALGSRC_CHORDALG_STRING_H__

#include <string>
#include <vector>
#include <utility>

namespace chordalg {

typedef std::vector< std::string > StringTokens;
typedef std::pair< std::string, std::string > StringPair;

// True iff str is a natural number
bool IsNum(const std::string&);

// Splits string and changes chars to lowercase
StringTokens Split(const std::string&, const std::string&);

}  // namespace chordalg

#endif  // CHORDALGSRC_CHORDALG_STRING_H__

