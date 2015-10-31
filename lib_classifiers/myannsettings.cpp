#include "myannsettings.h"

myAnnSettings::myAnnSettings()
{
    this->features_count = 0;
    this->classes_count = 0;
}

myAnnSettings::~myAnnSettings()
{
}

int myAnnSettings::getNumFeatures(){
    return this->vec_features.size();
}


int myAnnSettings::get_feature_vector_length(){
    return this->features_count;
}

int myAnnSettings::processFileName(string filename){

    vector<FeatureExtractor *> all_pointers;
    all_pointers.push_back(new HOGFeatures());
//    all_pointers.push_back(new BrightFeature());
    all_pointers.push_back(new experimentFeature());
//    all_pointers.push_back(new EdgeFeatures());
//    HistogramFeatures * fhisto  = new HistogramFeatures();
//    fhisto->setNumberOfBins(256);
//    all_pointers.push_back(fhisto);
//    all_pointers.push_back(new RawFeatures());
//    all_pointers.push_back(new LBPFeatures());
//    all_pointers.push_back(new SkeletFeatures());

//    all_pointers.push_back(new GrayScaleFeatures());
//    all_pointers.push_back(new CornerFeatures());
//    all_pointers.push_back(new maskfeatures());
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_TEETH));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_TONGUE));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_OPEN));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_DEFAULT));
    all_pointers.push_back(new IntegralFeature(IntegralFeature::CLASS_SMILE));

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
                    this->features_count += all_pointers[j]->count(); // add count
                }
            }
            cntFeatures++;
        }
        else if(part[0] >= 'A' || part[0] <= 'Z'){
            // number of classes
            this->classes_count++;
            this->classes.push_back(part);
        }
    }

    if(cntFeatures != vec_features.size()){
        cerr << "Some FEATURES are missing, or not loaded: " << filename << endl;
        return -1;
    }

    return 0;
}

void myAnnSettings::print(){
    cout << "Features: {";
    for(int i = 0; i < vec_features.size(); ++i){
        cout << vec_features[i]->name();
        cout << " [" << vec_features[i]->count() << "]";
        if(i < vec_features.size()-1){
            cout << ", ";
        }
    }
    if(vec_features.size() > 0){
        cout << " #" << vec_features.size() << " ["<< this->features_count << "]}";
    }
    cout << " Classes: {";
    for(int i = 0 ; i < classes.size(); ++i){
        cout << classes[i];
        if(i < classes.size()-1){
            cout <<", ";
        }
    }
    cout << "} #" << classes_count << endl;
}

void myAnnSettings::printFeatures(){

    for(int i = 0; i < vec_features.size(); ++i){
        cout << vec_features[i]->name();
        cout << " [" << vec_features[i]->count() << "]";
        if(i < vec_features.size()-1){
            cout << ", ";
        }
    }
    if(vec_features.size() > 0){
        cout << " #" << vec_features.size() << " ["<< this->features_count << "]"  << endl;
    }
}

string myAnnSettings::getFileName(string classifierSettings,int perc){
    string filename;
    for(int i = 0; i < classes.size(); ++i){
        filename.append(classes[i]);
            filename.append("_");
    }
    for(int i = 0 ; i < vec_features.size(); ++i){
        filename.append(vec_features[i]->name());
        if(i < vec_features.size()-1){
            filename.append("_");
        }
    }
    filename.append(classifierSettings);
    filename.append("-"+to_string(perc));
    filename.append(".yml");
    return filename;
}

