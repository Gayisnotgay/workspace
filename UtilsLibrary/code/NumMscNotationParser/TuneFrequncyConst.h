#pragma once

#include <map>
#include <tuple>

struct TuneFreq {
    int freq[3]; // 0-Low 1-Middle 2-High
};

// C大调：音名 (1-do 2-re 3-mi 4-fa 5-so 6-la 7-si) -> 频率[低，中，高] Hz
const std::map<std::string, TuneFreq> g_tuneCFreqList = {
    {"1",   {262, 523, 1046}},
    {"1.5", {277, 554, 1109}},
    {"2",   {294, 587, 1175}},
    {"2.5", {311, 622, 1245}},
    {"3",   {330, 659, 1318}},
    {"4",   {349, 698, 1397}},
    {"4.5", {370, 740, 1480}},
    {"5",   {392, 784, 1568}},
    {"5.5", {415, 831, 1661}},
    {"6",   {440, 880, 1760}},
    {"6.5", {466, 932, 1865}},
    {"7",   {494, 988, 1976}},
};

using TuneFreqMappingTuneCMap = std::map<std::string, std::tuple<std::string, int>>;

// C大调音名和C大调音名对应关系
// C大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneCmappingC = {
    {"L1", std::make_tuple("1",   0)},    
    {"L2", std::make_tuple("2",   0)},    
    {"L3", std::make_tuple("3",   0)},    
    {"L4", std::make_tuple("4",   0)},    
    {"L5", std::make_tuple("5",   0)},    
    {"L6", std::make_tuple("6",   0)},    
    {"L7", std::make_tuple("7",   0)},
    
    {"M1", std::make_tuple("1",   1)},    
    {"M2", std::make_tuple("2",   1)},    
    {"M3", std::make_tuple("3",   1)},    
    {"M4", std::make_tuple("4",   1)},    
    {"M5", std::make_tuple("5",   1)},    
    {"M6", std::make_tuple("6",   1)},    
    {"M7", std::make_tuple("7",   1)},

    {"H1", std::make_tuple("1",   2)},    
    {"H2", std::make_tuple("2",   2)},    
    {"H3", std::make_tuple("3",   2)},    
    {"H4", std::make_tuple("4",   2)},    
    {"H5", std::make_tuple("5",   2)},    
    {"H6", std::make_tuple("6",   2)},    
    {"H7", std::make_tuple("7",   2)},
};

// D大调音名和C大调音名对应关系
// D大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneDmappingC = {
    {"L1", std::make_tuple("2",   0)},    
    {"L2", std::make_tuple("3",   0)},    
    {"L3", std::make_tuple("4.5", 0)},    
    {"L4", std::make_tuple("5",   0)},    
    {"L5", std::make_tuple("6",   0)},    
    {"L6", std::make_tuple("7",   0)},    
    {"L7", std::make_tuple("1.5", 1)},
    
    {"M1", std::make_tuple("2",   1)},    
    {"M2", std::make_tuple("3",   1)},    
    {"M3", std::make_tuple("4.5", 1)},    
    {"M4", std::make_tuple("5",   1)},    
    {"M5", std::make_tuple("6",   1)},    
    {"M6", std::make_tuple("7",   1)},    
    {"M7", std::make_tuple("1.5", 2)},

    {"H1", std::make_tuple("2",   2)},
};

// E大调音名和C大调音名对应关系
// E大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneEmappingC = {
    {"L1", std::make_tuple("3",   0)},    
    {"L2", std::make_tuple("4.5", 0)},    
    {"L3", std::make_tuple("5.5", 0)},    
    {"L4", std::make_tuple("6",   0)},    
    {"L5", std::make_tuple("7",   0)},    
    {"L6", std::make_tuple("1.5", 1)},    
    {"L7", std::make_tuple("2.5", 1)},
    
    {"M1", std::make_tuple("3",   1)},    
    {"M2", std::make_tuple("4.5", 1)},    
    {"M3", std::make_tuple("5.5", 1)},    
    {"M4", std::make_tuple("6",   1)},    
    {"M5", std::make_tuple("7",   1)},    
    {"M6", std::make_tuple("1.5", 2)},    
    {"M7", std::make_tuple("2.5", 2)},

    {"H1", std::make_tuple("3",   2)},
};

// F大调音名和C大调音名对应关系
// F大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneFmappingC = {
    {"L1", std::make_tuple("4",   0)},    
    {"L2", std::make_tuple("5",   0)},    
    {"L3", std::make_tuple("6",   0)},    
    {"L4", std::make_tuple("6.5", 0)},    
    {"L5", std::make_tuple("1",   1)},    
    {"L6", std::make_tuple("2",   1)},    
    {"L7", std::make_tuple("3",   1)},
    
    {"M1", std::make_tuple("4",   1)},    
    {"M2", std::make_tuple("5",   1)},    
    {"M3", std::make_tuple("6",   1)},    
    {"M4", std::make_tuple("6.5", 1)},    
    {"M5", std::make_tuple("1",   2)},    
    {"M6", std::make_tuple("2",   2)},    
    {"M7", std::make_tuple("3",   2)},

    {"H1", std::make_tuple("4",   2)},
};

// G大调音名和C大调音名对应关系
// G大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneGmappingC = {
    {"L1", std::make_tuple("5",   0)},    
    {"L2", std::make_tuple("6",   0)},    
    {"L3", std::make_tuple("7",   0)},    
    {"L4", std::make_tuple("1",   1)},    
    {"L5", std::make_tuple("2",   1)},    
    {"L6", std::make_tuple("3",   1)},    
    {"L7", std::make_tuple("4.5", 1)},
    
    {"M1", std::make_tuple("5",   1)},    
    {"M2", std::make_tuple("6",   1)},    
    {"M3", std::make_tuple("7",   1)},    
    {"M4", std::make_tuple("1",   2)},    
    {"M5", std::make_tuple("2",   2)},    
    {"M6", std::make_tuple("3",   2)},    
    {"M7", std::make_tuple("4.5", 2)},

    {"H1", std::make_tuple("5",   2)},
};

// A大调音名和C大调音名对应关系
// A大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneAmappingC = {
    {"L1", std::make_tuple("6",   0)},    
    {"L2", std::make_tuple("7",   0)},    
    {"L3", std::make_tuple("1.5", 1)},    
    {"L4", std::make_tuple("2",   1)},    
    {"L5", std::make_tuple("3",   1)},    
    {"L6", std::make_tuple("4.5", 1)},    
    {"L7", std::make_tuple("5.5", 1)},
    
    {"M1", std::make_tuple("6",   1)},    
    {"M2", std::make_tuple("7",   1)},    
    {"M3", std::make_tuple("1.5", 2)},    
    {"M4", std::make_tuple("2",   2)},    
    {"M5", std::make_tuple("3",   2)},    
    {"M6", std::make_tuple("4.5", 2)},    
    {"M7", std::make_tuple("5.5", 2)},

    {"H1", std::make_tuple("6",   2)},
};

// B大调音名和C大调音名对应关系
// B大调音名 -> C大调<音名，挡位>
const TuneFreqMappingTuneCMap g_tuneBmappingC = {
    {"L1", std::make_tuple("7",   0)},    
    {"L2", std::make_tuple("1.5", 1)},    
    {"L3", std::make_tuple("2.5", 1)},    
    {"L4", std::make_tuple("3",   1)},    
    {"L5", std::make_tuple("4.5", 1)},    
    {"L6", std::make_tuple("5.5", 1)},    
    {"L7", std::make_tuple("6.5", 1)},
    
    {"M1", std::make_tuple("7",   1)},    
    {"M2", std::make_tuple("1.5", 2)},    
    {"M3", std::make_tuple("2.5", 2)},    
    {"M4", std::make_tuple("3",   2)},    
    {"M5", std::make_tuple("4.5", 2)},    
    {"M6", std::make_tuple("5.5", 2)},    
    {"M7", std::make_tuple("6.5", 2)},

    {"H1", std::make_tuple("7",   2)},
};
