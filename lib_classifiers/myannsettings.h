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
#include "lib_features/integralfeature.h"


using namespace std;

class myAnnSettings
{
public:
    myAnnSettings();
    ~myAnnSettings();
    int processFileName(string filename);
    int getNumFeatures();
    int get_feature_vector_length();

    string getFileName(string classifierSettings, int perc);

    void printFeatures();

    void print();

    vector<FeatureExtractor *> vec_features;
    vector<string> classes;
    int features_count;
    int classes_count;

};

#endif // MYANNSETTINGS_H
