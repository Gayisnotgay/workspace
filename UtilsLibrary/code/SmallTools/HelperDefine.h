#pragma once

#include <string>
#include <sstream>

#ifndef COUNTOF
#define COUNTOF(arr) \
    sizeof(arr) / sizeof(arr[0])
#endif

template<typename SequenceIterator>
std::string GetSequenceContents(SequenceIterator&& itBegin, SequenceIterator&& itEnd)
{
    std::ostringstream result(std::ios_base::ate);
    for(auto it = itBegin; it != itEnd; ++it) {
        result << *it << ",";
    }

    return result.str();
}