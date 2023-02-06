#pragma once
#include "MscAlphabet.h"
#include <vector>

class NumMscNotationParser {
public:
    NumMscNotationParser() = default;
    ~NumMscNotationParser() = default;
    int Parse(const std::vector<std::string>& mscNotation, std::vector<MscAlphabet::AlphabetProperty>& songData);
};