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
int myAnnSettings::getNumFeatures(){
    return this->vec_features.size();
}

string * myAnnSettings::getStrLabelsArray(){
    string label_array[this->vec_labels.size()];
    copy(this->vec_labels.begin(), this->vec_labels.end(), label_array);
    return label_array;
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
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_TEETH));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_TONGUE));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_OPEN));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_DEFAULT));

    filename = Support::getFileName(filename);
    vector<string> parts = Support::splitString(filename, '_');

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

void myAnnSettings::printClasses(){
    for(int i = 0; i < vec_labels.size(); ++i){
        cout << vec_labels[i];
        if(i < vec_labels.size()-1){
            cout << ", ";
        }
        else{
            cout << endl;
        }
    }
}

void myAnnSettings::printFeatures(){
    for(int i = 0; i < vec_features.size(); ++i){
        cout << vec_features[i]->name();
        if(i < vec_features.size()-1){
            cout << ", ";
        }
        else{
            cout << endl;
        }
    }
}
