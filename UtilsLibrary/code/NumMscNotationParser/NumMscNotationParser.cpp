#include "NumMscNotationParser.h"
#include <algorithm>
#include <memory>
#include "MessageCenter/MessageCenter.h"

using std::string;
using std::make_unique;
using std::for_each;
using std::vector;
using std::endl;

int NumMscNotationParser::Parse(const vector<string>& mscNotation, vector<MscAlphabet::AlphabetProperty>& songData)
{
    MelodyRuleParser mR;
    size_t alphabetOffset = mR.Parse(mscNotation);
    if (mscNotation.size() <= alphabetOffset) {
        MessageCenter::Instance() << "mscNotation's length is invalid: " << (alphabetOffset + 1) << endl;
        return EXIT_FAILURE;
    }

    auto prevAlpha = make_unique<MscAlphabet>();
    for_each(mscNotation.begin() + alphabetOffset, mscNotation.end(), [&](const string& notation) {
        MscAlphabet alph(*mR, notation, prevAlpha.get());
        alph.Parse();
        songData.push_back(alph.GetProperty());
        *prevAlpha = alph;
    });

    return EXIT_SUCCESS;
}
