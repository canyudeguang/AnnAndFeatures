#include "myannsettings.h"

myAnnSettings::myAnnSettings()
{
}

myAnnSettings::~myAnnSettings()
{
}

int myAnnSettings::getNumClasses(){
    return this->vec_labels.size();
}



int myAnnSettings::processFileName(string filename){

    vector<FeatureExtractor *> all_pointers;
    all_pointers.push_back(new HOGFeatures());
    all_pointers.push_back(new BrightFeature());
    all_pointers.push_back(new experimentFeature());
    all_pointers.push_back(new EdgeFeatures());
    HistogramFeatures * fhisto  = new HistogramFeatures();
    fhisto->setNumberOfBins(256);
    all_pointers.push_back(fhisto);
    all_pointers.push_back(new RawFeatures());
    all_pointers.push_back(new LBPFeatures());
    all_pointers.push_back(new SkeletFeatures());

    all_pointers.push_back(new GrayScaleFeatures());
    all_pointers.push_back(new CornerFeatures());
    all_pointers.push_back(new maskfeatures());

    filename = Support::getFileName(filename);
    vector<string> parts = Support::splitString(filename, '_');
    cout << filename << endl;

    int cntFeatures = 0;


    for(int i = 0; i < parts.size(); ++i){
        string part = parts[i];

        // Is Feature
        if(part[0] == 'f'){
            for(int j = 0; j < all_pointers.size(); ++j){
                if(part == all_pointers[j]->name()){
                    vec_features.push_back(all_pointers[j]);
                }
            }
            cntFeatures++;
        }
        // Is Label
        else if(part[0] >= 'A' && part[0] <= 'Z'){
            this->vec_labels.push_back(part);
        }
    }

    if(this->vec_labels.size() == 0){
        cerr << "No LABELS found in the filename: " << filename << endl;
        return -1;
    }
    if(cntFeatures != vec_features.size()){
        cerr << "Some FEATURES are missing, or not loaded: " << filename << endl;
        return -1;
    }

    return 0;
}
