#include <iostream>

using namespace std;

#include <algorithm>

#include "lib_support/support.h"
#include "lib_support/cvSupport.h"


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

#include "lib_features/featurespicker.h"

#include "lib_classifiers/ann.h"
#include "lib_classifiers/myann.h"
#include "lib_classifiers/myannsettings.h"

//#include "lib_classifiers/svm.h"
//#include "lib_classifiers/boostclass.h"
//#include "lib_classifiers/kn.h"
//#include "lib_classifiers/decisiontree.h"



void help(){
    cout << "Usage:" << endl;
    cout << "./AnnFeatures TrainDir TestDir -c Classifier {[Ann layers] classifier Settings} -f featureIndexes repetitions" << endl;
    cout << "f featureIndexes: fore example -f 0,1,4" << endl;
    cout << "{classifier Settings} :" << endl;
    cout << "\t ANN = {iter,BP_Coef,BP_Coef,[layout]}: " << endl;
    cout << "\t example -c ANN {1000,0.1,0.1,[2,4,6]} = " << "100 iterations, 0.1, 0.1 BP coef are default, 3 hidden layers with 2,4 and 6 neurons." << endl;
}

cv::Mat_<float> featureExtractionFromDir(vector<string> & train_images, vector<FeatureExtractor * > & vec_extractors, bool print = true){

    cv::Mat_<float> Features;
    for(uint i = 0; i < train_images.size(); ++i){
        cv::Mat img = imread(train_images[i],CV_LOAD_IMAGE_ANYCOLOR);

        // joined features from all extractors
        Mat_<float> fjoined;
        fjoined.setTo(0);


        for(uint j = 0; j < vec_extractors.size(); ++j){
            Mat_<float> f = vec_extractors[j]->getFeature(img);
            FeatureExtractor::joinFeatures(fjoined,f);
        }

        Features.push_back(fjoined); // add feature_vector to mat of all features
    }

    return Features;
}

void selectFeatures(vector<int> indexes, vector<FeatureExtractor * > & features){
    // All features;

    EdgeFeatures fEdge;
    experimentFeature fExper;
    RawFeatures fRaw;
    SkeletFeatures fSkelet;
    HOGFeatures fHog;
    BrightFeature fBright;
    LBPFeatures fLbp;
    maskfeatures fMask;
    GrayScaleFeatures fGS;
    CornerFeatures fCor;

    HistogramFeatures * fhisto  = new HistogramFeatures();
    fhisto->setNumberOfBins(256);

    vector<FeatureExtractor * > allPointers;
    allPointers.push_back(new EdgeFeatures());
    allPointers.push_back(new experimentFeature());
    //allPointers.push_back(fhisto);
    //allPointers.push_back(new RawFeatures());
    allPointers.push_back(new HOGFeatures());
    allPointers.push_back(new LBPFeatures());
    allPointers.push_back(new BrightFeature());
   // allPointers.push_back(new SkeletFeatures());

   // allPointers.push_back(new GrayScaleFeatures());
   // allPointers.push_back(new CornerFeatures());
    allPointers.push_back(new maskfeatures());


    // Select Features;
    for(int i = 0; i < allPointers.size();++i){
        for(int j = 0; j < indexes.size();++j){
            if(i == indexes[j]){
                features.push_back(allPointers[i]);
            }
        }

    }
}

int main(int argc, char *argv[]){

    if(argc > 5){
        if(string(argv[3]) == "-c"){
            Classifier * classifier;
            if(string(argv[4]) == Classifier::C_ANN){

/// Cosntant functions
                // Setting repetitions
                int repetitions = 10;
                if(argc == 9) repetitions = stoi(argv[8]);

                if(string(argv[6]) == "-f"){
                    string strSettings(argv[7]);
                    myAnnSettings annSettings;
                    annSettings.processFileName(strSettings);

                    // Load Train from Directory
                    string directory(argv[1]);
                    vector<string> train_images = Support::pathVector(directory,".jpg");
                    cout << "From: " << directory << " "<< train_images.size() << " images loaded." << endl;
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

                    // Predict & testing
                    string test_dir(argv[2]);
                    cout << "Predict ... from " << test_dir << endl;
                    vector<string> test_imgs = Support::pathVector(test_dir,".jpg");
                    sort(test_imgs.begin(), test_imgs.end());
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

                    // Feature extraction
                    cv::Mat_<float> test_features = featureExtractionFromDir(test_imgs,annSettings.vec_features);
                    cv::Mat_<float> Features = featureExtractionFromDir(train_images,annSettings.vec_features);
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


        /////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                        /// Training
                        for(int REPEAT = 0; REPEAT < repetitions; ++REPEAT){
                            classifier = new myANN();
                            // Set Classifier
                            classifier->setFeatureVectorSize(Features.cols);

                            // Lebel extraction
                            string str_labels[annSettings.vec_labels.size()];
                            copy(annSettings.vec_labels.begin(), annSettings.vec_labels.end(), str_labels);
                            int numClasses = annSettings.getNumClasses();
                            classifier->setLabels(str_labels,numClasses);

                            vector<uchar> eLabels = classifier->extLabelFromFileName(train_images);

                            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                            cout << "Classes: " << classifier->getStrLabels() << endl;
                            // set Classifier Params
                            classifier->loadFromParams(string(argv[5]));
                            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


                            // Training
                            if(eLabels.size() == Features.rows){
                                classifier->train(Features, eLabels, numClasses);
                            }
                            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                            // Prediction and Evaluation
                            vector<uchar> predictions = classifier->predict(test_features);
                            vector<uchar> test_labels = classifier->extLabelFromFileName(test_imgs);
                            double perc = classifier->evaluateVerbose(predictions,test_labels,numClasses);
                            classifier->evaluate(predictions,test_labels,numClasses);
                            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

                            //Saving
                            string toSave = "nets/"+ classifier->getStrLabels() + "_";
                            string features ="";
                            for(int i = 0; i < annSettings.vec_features.size();++i){
                                features += annSettings.vec_features[i]->name() + "_";
                            }
                            toSave += features;
                            toSave += classifier->getStrSettings() + "_";
                            toSave += to_string(int(perc)) + "_" + to_string(REPEAT) + ".yml";

                            if(perc > 85){
                                classifier->save2file(toSave.c_str());
                            }
                            else{
                                cout  << toSave << endl;
                            }
                            cout << "FEATURES: " << features << endl;
                            cout << "------------------------------------------" << endl;
                            delete classifier;
                        }

                }// END -f arg
                else{
                    help();
                }

            }


        }
    }
    else if(argc == 4){
        string testDir(argv[1]);
        string param(argv[2]);
        if(param == "-n"){
            string netPath(argv[3]);

            // Get ANN settings from the string;
            myAnnSettings annSettings;
            annSettings.processFileName(netPath);

            // set Labels
            string label_array[annSettings.vec_labels.size()];
            copy(annSettings.vec_labels.begin(), annSettings.vec_labels.end(), label_array);
            int numClasses = annSettings.getNumClasses();

            // Load images
            vector<string> test_imgs = Support::pathVector(testDir,".jpg");

            // Load classifier
            Classifier * classifier = new myANN();
            classifier->setLabels(label_array,numClasses);
            classifier->loadFromFile(netPath.c_str());

            // get Features from Images
            cv::Mat_<float> test_features = featureExtractionFromDir(test_imgs,annSettings.vec_features);
            classifier->setFeatureVectorSize(test_features.cols);
            //Prediction
            vector<uchar> predictions = classifier->predict(test_features);
            vector<uchar> test_labels = classifier->extLabelFromFileName(test_imgs);
            classifier->evaluate(predictions,test_labels,numClasses);
        }
    }
    else{
        help();
    }
    return 0;
}

