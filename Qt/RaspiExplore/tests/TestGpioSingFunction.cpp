#include <tuple>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GpioFunctions/GpioSingFunction.h"

using namespace testing;
using std::string;
using std::vector;

TEST(GpioSingFunction, ParseParamShouldReturnSuccessWhenInputSongFile)
{
    SingleGpioToneConfig cfg;
    int parseResult = GpioSingFunction().ParseParam({"8", "..//happybirthdayV2.song"}, cfg);
    ASSERT_TRUE(parseResult == EXIT_SUCCESS);
    EXPECT_THAT(cfg.offset, 8);
    vector<SingleGpioToneConfig::Pwm>  expSongWaveData = {
        {523,  250}, {523,  250}, {587,  500}, {523,  500}, {698,  500}, {659,  500}, {659,  500},
        {523,  250}, {523,  250}, {587,  500}, {523,  500}, {784,  500}, {698,  500}, {698,  500},
        {523, 500},  {523, 500},  {1046, 500}, {880,  500}, {698,  500}, {659,  500}, {587,  500},
        {932, 250},  {932,  250}, {880,  500}, {698,  500}, {784,  500}, {698,  500}, {698,  500},
    };
    EXPECT_THAT(cfg.tones, expSongWaveData);
}