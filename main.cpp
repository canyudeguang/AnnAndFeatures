#include <iostream>
#include <algorithm>

#include "lib_support/support.h"
#include "lib_support/cvSupport.h"

#include "lib_features/featurespicker.h"

#include "lib_classifiers/myann.h"
#include "lib_classifiers/myannsettings.h"

#include "lib_classifiers/labelextractor.h"

using namespace std;
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

#define INFO



int main(int argc, char *argv[]){

    if(argc > 5){
        if(string(argv[3]) == "-c"){
            Classifier * classifier;
             ///@TODO set SVM classifier
            if(string(argv[4]) == Classifier::C_ANN){
                // Creation of the classifier Object
                classifier = new myANN();
#ifdef INFO
                cout << "Classifier selected: " << classifier->name() << endl;
#endif

                // Load Train and Test Images
                string directory(argv[1]);
                vector<string> train_images = Support::pathVector(directory,".jpg");
#ifdef INFO
                cout << "Training from: \t" << directory << "\t" << train_images.size() << " images loaded." << endl;
#endif
                string test_dir(argv[2]);
                vector<string> test_imgs = Support::pathVector(test_dir,".jpg");
#ifdef INFO
                cout << "Predict from: \t" << test_dir << "\t" << test_imgs.size() << " images loadad." << endl;
#endif
                sort(test_imgs.begin(), test_imgs.end());

                string strLabelsFeatures(argv[7]);
                // Label extraction
                LabelExtractor Test_Labels;
                Test_Labels.setLabels(strLabelsFeatures);
                cout << Test_Labels.printLblNames(false) << endl;
                Test_Labels.extractLabelsFromFiles(test_imgs);\
                Test_Labels.printAll();

                exit(0);

                vector<uchar> train_labels = classifier->extLabelFromFileName(train_images);
                vector<uchar> test_labels = classifier->extLabelFromFileName(test_imgs);


/// Cosntant functions
                ///@TODO erase! - I guess virtual destructor insteaf
                // Setting repetitions
                int repetitions = 6;
                if(argc == 9) repetitions = stoi(argv[8]);

                // Set Features and Classes
                // format : DEFAULT_REST_fFeature1_fFeature2
                // CAPITALS are classest
                // fFeatureName are features to extract

                if(string(argv[6]) == "-f"){

                    string strSettings(argv[7]);
                    myAnnSettings annSettings;

                    if( annSettings.processFileName(strSettings) != -1){
#ifdef INFO
                        cout << "Classes:\t";
                        annSettings.printClasses();
                        cout << "Features:\t";
                        annSettings.printFeatures();
#endif
                    }


                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Feature extraction
                    cv::Mat_<float> test_features = featureExtractionFromDir(test_imgs,annSettings.vec_features);
                    cv::Mat_<float> Features = featureExtractionFromDir(train_images,annSettings.vec_features);
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Training
                    ///@TODO repetitions
                    for(int REPEAT = 0; REPEAT < repetitions; ++REPEAT){

                    classifier = new myANN();
                    classifier->setLabels(annSettings.vec_labels);
                    classifier->setFeatureVectorSize(Features.cols);



                    // set Classifier Params - from commandline
                    // -c ANN {100,0.1,0.1[20]}
                    // 100 iters; 0.1, 0.1 alpha, beta; 20 neurons in the first layer, 2 in the second
                    classifier->loadFromParams(string(argv[5]));
                    if(train_labels.size() == Features.rows){
                        classifier->train(Features, train_labels);
                    }
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Prediction and Evaluation
                    vector<uchar> predictions = classifier->predict(test_features);
                    double perc = classifier->evaluateVerbose(predictions,test_labels);
                    //classifier->printLabels(test_imgs,test_labels);
                    classifier->evaluate(predictions,test_labels,classifier->numberOfClasses);
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Saving the net:  Up 85 > Save
                    string toSave = "nets/"+ classifier->getStrLabels() + "_";
                    string features ="";
                    for(int i = 0; i < annSettings.vec_features.size();++i){
                        features += annSettings.vec_features[i]->name() + "_";
                    }
                    toSave += features;
                    toSave += classifier->getStrSettings() + "_";
                    toSave += to_string(int(perc)) + "_" + to_string(0) + ".yml";
                    if(perc > 72){
                        classifier->save2file(toSave.c_str());
                    }
                    else{
                        cout  << toSave << endl;
                    }

                    delete classifier;}
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

