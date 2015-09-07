#ifndef FEATURESPICKER_H
#define FEATURESPICKER_H

#include <vector>
#include "lib_features/featureextractor.h"
#include "lib_support/support.h"

class FeaturesPicker
{
public:
    FeaturesPicker();
    ~FeaturesPicker();

    vector<vector <int> > powerSetIndexes(int size);
    static vector<int> getIndexesFromArguments(string args);
};

#endif // FEATURESPICKER_H
