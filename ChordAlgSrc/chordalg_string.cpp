#include "chordalg_string.h"

namespace chordalg {

// True iff str is a natural number
bool IsNum(std::string str) {
    for (auto c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return str.empty() ? false : true;
}

// Splits string and changes chars to lowercase
StringTokens Split(std::string str, std::string delim) {
    StringTokens tokens;
    char* pch = nullptr;
    char* cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    pch = strtok(cstr, delim.c_str());
    while (pch) {
        std::string tok(pch);
        for (size_t i = 0; i < tok.length(); ++i) {
            if (isalpha(tok[i]) && !islower(tok[i])) {
                tok[i] = tolower(tok[i]);
            }
        }
        tokens.push_back(tok);
        pch = strtok(nullptr, delim.c_str());
    }
    delete cstr;
    return tokens;
}

}  // namespace ChordAlg