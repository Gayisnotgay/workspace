#include "MscAlphabet.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <map>
#include <tuple>
#include <cmath>
#include "ParseConfig/ParseConfig.h"
#include "MessageCenter/MessageCenter.h"
#include "TuneFrequncyConst.h"
#include "SmallTools/AlmostEqual.h"

using std::cout;
using std::endl;
using std::string;
using std::stof;
using std::map;
using std::vector;
using std::function;
using std::for_each;
using std::tuple;
using std::make_tuple;
using std::get;


const map<string, MelodyTune> g_melodyTuneMap = {
    {"C", MelodyTune::C},
    {"D", MelodyTune::D},
    {"E", MelodyTune::E},
    {"F", MelodyTune::F},
    {"G", MelodyTune::G},
    {"A", MelodyTune::A},
    {"B", MelodyTune::B},
};



const map<MelodyTune, TuneFreqMappingTuneCMap> freqPasswordBooks = {
    {MelodyTune::C, g_tuneCmappingC},
    {MelodyTune::D, g_tuneDmappingC},
    {MelodyTune::E, g_tuneEmappingC},
    {MelodyTune::F, g_tuneFmappingC},
    {MelodyTune::G, g_tuneGmappingC},
    {MelodyTune::A, g_tuneAmappingC},
    {MelodyTune::B, g_tuneBmappingC}
};

bool MelodyRule::operator==(const MelodyRule& obj) const
{
    return IsAlmostEqual(obj.durationOneBeat, durationOneBeat) && (obj.tune == tune);
}

size_t MelodyRuleParser::Parse(const vector<string>& mscNotation)
{
    // 乐谱最短长度: 基调[1=F] 小节配置[3/4] 每一拍时长(毫秒)[500]
    constexpr size_t MIN_MELODYRULE_LENGTH = 3;
    if (mscNotation.size() < MIN_MELODYRULE_LENGTH) {
        MessageCenter::Instance() << "invalid mscNotation length, atleast 3 (eg: 1=C 3/4 500):" << endl;
        return std::numeric_limits<size_t>::max();
    }

    auto melodyTune = ParseConfig::ParseAssignCfg(mscNotation[0]);
    auto it = g_melodyTuneMap.find(melodyTune.begin()->second);
    if (it == g_melodyTuneMap.end()) {
        MessageCenter::Instance() << "unknown tuneFlag:" << melodyTune.begin()->second << ", default to Tune C" << endl;
        return std::numeric_limits<size_t>::max();
    }
    m_rule.tune = it->second;
    m_rule.durationOneBeat = stof(mscNotation[2]);

    return MIN_MELODYRULE_LENGTH;
}

MelodyRule MelodyRuleParser::operator*() const
{
    return m_rule;
}

void MscAlphabet::Parse()
{
    MscAlphabetParseHelper(*this).Parse();
}

MscAlphabet::AlphabetProperty MscAlphabet::GetProperty()
{
    return m_property;
}

bool MscAlphabet::AlphabetProperty::operator==(const MscAlphabet::AlphabetProperty& obj) const
{
    return IsAlmostEqual(obj.toneFreq, toneFreq) && IsAlmostEqual(obj.toneDuration, toneDuration);
}

std::ostream& operator<<(std::ostream& os, const MscAlphabet::AlphabetProperty& obj)
{
    os << obj.toneFreq << "," << obj.toneDuration << " ";
    return os;
}

int MscAlphabetParseHelper::Parse()
{
    vector<function<int()>> parseSteps = {
        [this]() { return ParseMainAlphabetNumber(); },
        [this]() { return ParseAlphabetLevel(); },
        [this]() { return ParseAlphabetDuration(); },
    };

    for(auto& parseStep: parseSteps) { 
        if (parseStep() != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseMainAlphabetNumber()
{
    if (m_obj.m_rawAlphaStr.empty()) {
        return EXIT_FAILURE;
    }

    vector<tuple<function<bool()>, function<int()>>> parseSteps = {
        // 解析1~7的音阶数字
        {[&](){ return isdigit(m_obj.m_rawAlphaStr[0]); }, [&](){ return ParseMetaNumber(m_obj.m_rawAlphaStr[0]); }},
        // 解析单独的-
        {[&](){ return (m_obj.m_rawAlphaStr[0] == '-'); }, [&](){ return ParseSlash(); }},
        // 解析单独的.
        {[&](){ return (m_obj.m_rawAlphaStr[0] == '.'); }, [&](){ return ParseDot(); }},
        // 解析单独的S(模拟连音符中间的停顿)
        {[&](){ return (m_obj.m_rawAlphaStr[0] == 'S'); }, [&](){ return ParseSlurBreak(); }},
    };

    for (auto& step : parseSteps) {
        if (get<0>(step)()) {
            return get<1>(step)();
        }
    }

    MessageCenter::Instance() << "ParseMainAlphabetNumber failed due to invalid format(should at least have number,-,.): " << m_obj.m_rawAlphaStr << endl;

    return EXIT_FAILURE;
}

int MscAlphabetParseHelper::ParseAlphabetLevel()
{
    if ((m_alphabet.length() != 1) || (m_obj.GetProperty().toneFreq != 0)) {
        return EXIT_SUCCESS;
    }

    if (m_obj.m_rawAlphaStr.find("-") != string::npos) {
        m_alphabet.insert(0, "L");
    } else if (m_obj.m_rawAlphaStr.find("+") != string::npos) {
        m_alphabet.insert(0, "H");
    } else {
        m_alphabet.insert(0, "M");
    }

    auto& passwordBook = freqPasswordBooks.at(m_obj.m_rule.tune);
    if (passwordBook.find(m_alphabet) == passwordBook.end()) {
        MessageCenter::Instance() << m_alphabet << "not exist" << endl;
        return EXIT_FAILURE;
    }

    auto& mappingKeys = passwordBook.at(m_alphabet);
    auto mappingKeyRow = get<0>(mappingKeys);
    auto mappingKeyColumn = get<1>(mappingKeys);
    m_obj.m_property.toneFreq = g_tuneCFreqList.at(mappingKeyRow).freq[mappingKeyColumn];

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseAlphabetDuration()
{
    if (/*m_alphabet.empty() || */(m_obj.GetProperty().toneDuration != 0)) {
        return EXIT_SUCCESS;
    }

    auto underlineList = ParseConfig::ParsePattern(m_obj.m_rawAlphaStr, "(_)");
    m_obj.m_property.toneDuration = m_obj.m_rule.durationOneBeat / std::pow(2, underlineList.size());

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseMetaNumber(char num)
{
    // 处理特殊的休止符0
    if (num == '0') {
        m_obj.m_property.toneFreq = 0.01;
        return EXIT_SUCCESS;
    }

    if (num < '1' || num > '7') {
        MessageCenter::Instance() << "invalid AlphabetNumber:" << num << ", should be 1~7" << endl;
        return EXIT_FAILURE;
    }
    m_alphabet.append({num});

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseSlash()
{
    if (m_obj.m_rawAlphaStr != "-") {
        MessageCenter::Instance() << "ParseSlash error, unexpected slash-" << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    if (m_obj.m_prev == nullptr) {
        MessageCenter::Instance() << "ParseSlash error, invalid slash-,m_prev is nullptr" << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    m_obj.m_property = m_obj.m_prev->m_property;

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseDot()
{
    if (m_obj.m_rawAlphaStr != ".") {
        MessageCenter::Instance() << "ParseSlash error, unexpected dot." << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    if (m_obj.m_prev == nullptr) {
        MessageCenter::Instance() << "ParseSlash error, m_prev is nullptr" << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    m_obj.m_property.toneFreq = m_obj.m_prev->m_property.toneFreq;
    m_obj.m_property.toneDuration = m_obj.m_prev->m_property.toneDuration / 2;

    return EXIT_SUCCESS;
}

int MscAlphabetParseHelper::ParseSlurBreak()
{
    if (m_obj.m_rawAlphaStr != "S") {
        MessageCenter::Instance() << "ParseShortBreak error, unexpected S." << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    if (m_obj.m_prev == nullptr) {
        MessageCenter::Instance() << "ParseShortBreak error, m_prev is nullptr" << m_obj.m_rawAlphaStr << endl;
        return EXIT_FAILURE;
    }

    m_obj.m_property.toneFreq = 0.1;
    m_obj.m_property.toneDuration = m_obj.m_prev->m_property.toneDuration / 10;

    return EXIT_SUCCESS;
}