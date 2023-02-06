#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "NumMscNotationParser/MscAlphabet.h"

using namespace testing;
using std::string;
using std::map;

TEST(MelodyRuleParser, ShouldReturnCorrectMelodayWhenInput1eqX)
{
    MelodyRuleParser mR;
    size_t ruleLength = mR.Parse({"1=C", "", "500"});
    EXPECT_THAT(ruleLength, 3);

    MelodyRule expParsedRule{MelodyTune::C, 500};
    EXPECT_THAT(*mR, expParsedRule);
    // EXPECT_THAT(*MelodyRuleParser("1=D"), MelodyTune::D);
    // EXPECT_THAT(*MelodyRuleParser("1=E"), MelodyTune::E);
    // EXPECT_THAT(*MelodyRuleParser("1=F"), MelodyTune::F);
    // EXPECT_THAT(*MelodyRuleParser("1=G"), MelodyTune::G);
    // EXPECT_THAT(*MelodyRuleParser("1=A"), MelodyTune::A);
    // EXPECT_THAT(*MelodyRuleParser("1=B"), MelodyTune::B);
}

TEST(MscAlphabet, ShouldOutput523Hzand1000msWhenInputCtuneandAlphabet1)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{523, 1000};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput262Hzand1000msWhenInputCtuneandAlphabet1minus)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1-", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{262, 1000};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput1046Hzand1000msWhenInputCtuneandAlphabet1plus)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1+", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{1046, 1000};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput523Hzand500msWhenInputCtuneandAlphabet1underline)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{523, 500};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput523Hzand500msWhenInputCtuneandAlphabet1minusunderline)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1-_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{262, 500};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput1046Hzand500msWhenInputCtuneandAlphabet1plusunderline)
{
    MscAlphabet alphabet({MelodyTune::C, 1000}, "1+_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{1046, 500};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput1046Hzand250msWhenInputCtuneandAlphabet1plusunderlineandAlphabetdot)
{
    MscAlphabet alphabet1({MelodyTune::C, 1000}, "1+_", nullptr);
    MscAlphabet alphabet2({MelodyTune::C, 1000}, ".", &alphabet1);
    alphabet1.Parse();
    alphabet2.Parse();
    MscAlphabet::AlphabetProperty expProperty2{1046, 250};
    EXPECT_DOUBLE_EQ(alphabet2.GetProperty().toneFreq, expProperty2.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet2.GetProperty().toneDuration, expProperty2.toneDuration);
}

TEST(MscAlphabet, ShouldOutput1046Hzand500msWhenInputCtuneandAlphabet1plusunderlineandAlphabetslash)
{
    MscAlphabet alphabet1({MelodyTune::C, 1000}, "1+_", nullptr);
    MscAlphabet alphabet2({MelodyTune::C, 1000}, "-", &alphabet1);
    alphabet1.Parse();
    alphabet2.Parse();
    MscAlphabet::AlphabetProperty expProperty2{1046, 500};
    EXPECT_DOUBLE_EQ(alphabet2.GetProperty().toneFreq, expProperty2.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet2.GetProperty().toneDuration, expProperty2.toneDuration);
}

TEST(MscAlphabet, ShouldOutput0Hzand0msWhenInputFtuneandAlphabet5plnusunderline)
{
    MscAlphabet alphabet({MelodyTune::F, 1000}, "5+_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{0, 0};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput523Hzand500msWhenInputFtuneandAlphabet5minusunderline)
{
    MscAlphabet alphabet({MelodyTune::F, 1000}, "5-_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{523, 500};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}

TEST(MscAlphabet, ShouldOutput1397Hzand500msWhenInputFtuneandAlphabet1plusunderline)
{
    MscAlphabet alphabet({MelodyTune::F, 1000}, "1+_", nullptr);
    alphabet.Parse();
    MscAlphabet::AlphabetProperty expProperty{1397, 500};
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneFreq, expProperty.toneFreq);
    EXPECT_DOUBLE_EQ(alphabet.GetProperty().toneDuration, expProperty.toneDuration);
}