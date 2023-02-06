#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "NumMscNotationParser/NumMscNotationParser.h"

using namespace testing;
using std::string;
using std::map;
using std::vector;

TEST(NumMscNotationParser, ShouldReturnFailureWhenInputDataLengthIsLessThan2)
{
    vector<MscAlphabet::AlphabetProperty> songData;
    int parseResult = NumMscNotationParser().Parse({"1=F", "", "1000"}, songData);
    EXPECT_TRUE(parseResult != EXIT_SUCCESS);
    EXPECT_TRUE(songData.empty());
}

TEST(NumMscNotationParser, ShouldReturnSuccessWhenInputSongFileReceip)
{
    vector<std::string> mscNotation = {
        "1=F", "3/4", "1000",
        "5-_", "5-_", "6-", "5-", "1", "7-", "-", 
        "5-_", "5-_", "6-", "5-", "2", "1", "-", 
        "5-", "5-", "5", "3", "1", "7-", "6-", 
        "4", ".", "4_", "3", "1", "2", "1", "-",
        "5_", "S", "0__"
    };
    vector<MscAlphabet::AlphabetProperty> songData;
    vector<MscAlphabet::AlphabetProperty> expSongData = {
        {523,  500}, {523,  500}, {587,  1000}, {523,  1000}, {698,  1000}, {659,  1000}, {659,  1000},
        {523,  500}, {523,  500}, {587,  1000}, {523,  1000}, {784,  1000}, {698,  1000}, {698,  1000},
        {523, 1000}, {523, 1000}, {1046, 1000}, {880,  1000}, {698,  1000}, {659,  1000}, {587,  1000},
        {932, 1000}, {932,  500}, {932,   500}, {880,  1000}, {698,  1000}, {784,  1000}, {698,  1000}, {698,  1000},
        {1046, 500}, {0.1,  50}, {0.01,  250}  
    };
    int parseResult = NumMscNotationParser().Parse(mscNotation, songData);
    ASSERT_TRUE(parseResult == EXIT_SUCCESS);
    EXPECT_THAT(songData, expSongData);
}
