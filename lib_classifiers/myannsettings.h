#ifndef MYANNSETTINGS_H
#define MYANNSETTINGS_H

#include <string>
#include <vector>


#include "lib_support/support.h"
#include "lib_features/featureextractor.h"

#include "lib_features/histogramfeatures.h"
#include "lib_features/cornerfeatures.h"
#include "lib_features/edgefeatures.h"
#include "lib_features/experimentfeature.h"
#include "lib_features/rawfeatures.h"
#include "lib_features/skeletfeatures.h"
#include "lib_features/hogfeatures.h"
#include "lib_features/lbpfeatures.h"
#include "lib_features/brightfeature.h"
#include "lib_features/maskfeatures.h"
#include "lib_features/grayscalefeatures.h"


using namespace std;

class myAnnSettings
{
public:
    myAnnSettings();
    ~myAnnSettings();
    int processFileName(string filename);
    int getNumClasses();

    vector<string> vec_labels;
    vector<FeatureExtractor *> vec_features;
};

#endif // MYANNSETTINGS_H
