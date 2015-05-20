#include "ChordAlgSrc/chordalg_string.h"

#include <cstring>
#include <string>

namespace chordalg {

// True iff str is a natural number
bool IsNum(const std::string& str) {
    for (auto c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return str.empty() ? false : true;
}

// Splits string and changes chars to lowercase
StringTokens Split(const std::string& str, const std::string& delim) {
    StringTokens tokens;
    char* pch = nullptr;
    char* cstr = new char[str.length() + 1];
    char* saveptr;
    std::snprintf(cstr, str.length() + 1, "%s", str.c_str());
    pch = strtok_r(cstr, delim.c_str(), &saveptr);
    while (pch) {
        std::string tok(pch);
        for (size_t i = 0; i < tok.length(); ++i) {
            if (isalpha(tok[i]) && !islower(tok[i])) {
                tok[i] = tolower(tok[i]);
            }
        }
        tokens.push_back(tok);
        pch = strtok_r(nullptr, delim.c_str(), &saveptr);
    }
    delete cstr;
    return tokens;
}

}  // namespace chordalg
