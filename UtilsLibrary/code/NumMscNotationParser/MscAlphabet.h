#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class MelodyTune {
    C,
    D,
    E,
    F,
    G,
    A,
    B,
};

struct MelodyRule {
    MelodyTune tune;
    double durationOneBeat;
    bool operator==(const MelodyRule& obj) const;
};

class MelodyRuleParser {
public:
    MelodyRuleParser() = default;
    ~MelodyRuleParser() = default;
    // 返回值表示MelodyRule在mscNotation中的长度
    size_t Parse(const std::vector<std::string>& mscNotation);
    MelodyRule operator*() const;

private:
    MelodyRule m_rule = { MelodyTune::C, 1000 };
};

class MscAlphabet {
public:
    MscAlphabet(const MelodyRule &rule, const std::string& alphabet, MscAlphabet* prev) : m_rule(rule), m_rawAlphaStr(alphabet), m_prev(prev) {}
    MscAlphabet() = default;
    virtual ~MscAlphabet() = default;
    struct AlphabetProperty {
        double toneFreq;       // Hz
        double toneDuration;   // millisec
        bool operator==(const AlphabetProperty& obj) const;
        friend std::ostream& operator<<(std::ostream& os, const AlphabetProperty& obj);
    };

    void Parse();
    AlphabetProperty GetProperty();
    friend class MscAlphabetParseHelper;
private:
    MelodyRule m_rule = { MelodyTune::C, 1000 };
    std::string m_rawAlphaStr = "";
    MscAlphabet* m_prev = nullptr;

    AlphabetProperty m_property = {0, 0};
};

class MscAlphabetParseHelper {
public:
    MscAlphabetParseHelper(MscAlphabet& obj) : m_obj(obj) {}
    ~MscAlphabetParseHelper() = default;
    int Parse();
private:
    int ParseMainAlphabetNumber();
    int ParseAlphabetLevel();
    int ParseAlphabetDuration();

    int ParseMetaNumber(char num);
    int ParseSlash();
    int ParseDot();
    int ParseSlurBreak();
    MscAlphabet& m_obj;
    std::string m_alphabet;
};