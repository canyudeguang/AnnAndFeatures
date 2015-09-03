#ifndef FEATURESPICKER_H
#define FEATURESPICKER_H

#include <vector>
#include <lib_features/featureextractor.h>

class FeaturesPicker
{
public:
    FeaturesPicker();
    ~FeaturesPicker();

    vector<vector <int> > powerSetIndexes(int size);
};

#endif // FEATURESPICKER_H
