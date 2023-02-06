#include "ParseConfig.h"
#include <regex>

using std::map;
using std::vector;
using std::string;
using std::regex;
using std::regex_search;
using std::smatch;

map<string, string> ParseConfig::ParseAssignCfg(const string& cfg)
{
    regex re("\\s*(\\S+)\\s*=\\s*(\\S+)");
    smatch matchResult;
    bool isMatch = true;
    string source(cfg);
    map<string, string> parsedCfg;
    while (isMatch) {
        isMatch = regex_search(source, matchResult, re);
        if (isMatch && (matchResult.size() == 3)) {
            parsedCfg.emplace(matchResult[1].str(), matchResult[2].str());
            source = matchResult.suffix().str();
        } else {
            isMatch = false;
            break;
        }
    }

    return parsedCfg;
}

vector<string> ParseConfig::SplitCfgStr(const string& cfg)
{
    regex re("\\s*(\\w+)\\s*,?");
    smatch matchResult;
    bool isMatch = true;
    string source(cfg);
    vector<string> parsedCfg;
    while (isMatch) {
        isMatch = regex_search(source, matchResult, re);
        if (isMatch && (matchResult.size() == 2)) {
            parsedCfg.emplace_back(matchResult[1].str());
            source = matchResult.suffix().str();
        } else {
            isMatch = false;
            break;
        }
    }

    return parsedCfg;
}

// 按照输入的正则表达式循环解析
std::vector<std::string> ParseConfig::ParsePattern(const string& cfg, const std::string& regPattern)
{
#if (__cplusplus > 201703L)
    // regex re(regPattern, std::regex_constants::ECMAScript | std::regex_constants::multiline);
    regex re(regPattern, std::regex_constants::ECMAScript);
#else
    regex re(regPattern, std::regex_constants::ECMAScript);
#endif
    smatch matchResult;
    bool isMatch = true;
    string source(cfg);
    vector<string> parsedCfg;
    while (isMatch) {
        isMatch = regex_search(source, matchResult, re);
        if (isMatch && (matchResult.size() == 2)) {
            parsedCfg.emplace_back(matchResult[1].str());
            source = matchResult.suffix().str();
        } else {
            isMatch = false;
            break;
        }
    }

    return parsedCfg;  
}