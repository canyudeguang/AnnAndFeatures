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


#define INFO

int main(int argc, char *argv[]){

    if(argc > 5){
        if(string(argv[3]) == "-c"){
            Classifier * classifier;
             ///@TODO set SVM classifier
            if(string(argv[4]) == Classifier::C_ANN){
                //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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
                sort(train_images.begin(), train_images.end());

                if(string(argv[6]) == "-f"){
                    string strSettings(argv[7]);
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Label extraction
                    LabelExtractor Test_Labels(strSettings, test_imgs);
                    LabelExtractor Train_Labels(strSettings, train_images);

                    Train_Labels.removeUnknowns(train_images);
#ifdef INFO
                    cout << "Train ";
                    Train_Labels.printLblNames();
                    cout << "Test ";
                    Test_Labels.printLblNames();
#endif
                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Settings of Classifier
                    myAnnSettings annSettings;
                    if (annSettings.processFileName(strSettings) != -1){
#ifdef INFO
                        cout << "AnnSettings:\t";
                        annSettings.print();
#endif
                    }

                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Feature extraction
                    cv::Mat_<float> test_features = FeatureExtractor::extractFromImgs(test_imgs,annSettings.vec_features);
                    cv::Mat_<float> train_features = FeatureExtractor::extractFromImgs(train_images, annSettings.vec_features);
                    // Setting repetitions
                    int repetitions = 6;
                    if(argc == 9) repetitions = stoi(argv[8]);

                    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                    // Training
                    // Creation of the classifier Object
                    classifier = new myANN();
#ifdef INFO
                    cout << "Classifier selected: " << classifier->name() << endl;
#endif
                    string annParams(argv[5]);
                    classifier->setClassesVecLen(annSettings.classes_count, annSettings.features_count);
                    classifier->loadFromParams(annParams);
                    classifier->train(train_features, Train_Labels.mLabels);
                    vector<uchar> predictLabels = classifier->predict(test_features);

                    classifier->evaluate(predictLabels, Test_Labels.mLabels, Test_Labels.number_of_classes);
                    double perc = classifier->evaluateVerbose(predictLabels,Test_Labels.mLabels);

                    string filename = annSettings.getFileName(classifier->getStrSettings(),perc);
                    classifier->save2file(filename.c_str());

                    delete classifier;

                    myANN loadedClass;
                    loadedClass.loadFromFile(filename.c_str());
                    cout << loadedClass.getStrSettings() << endl;

                    cv::Mat_<float> f = test_features.row(1);
                    vector<uchar> p = loadedClass.predict(f);
                cout << "adad: " <<(int)p[0] << endl;


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
//            string netPath(argv[3]);

//            // Get ANN settings from the string;
//            myAnnSettings annSettings;
//            annSettings.processFileName(netPath);

//            // set Labels
//            string label_array[annSettings.vec_labels.size()];
//            copy(annSettings.vec_labels.begin(), annSettings.vec_labels.end(), label_array);
//            int numClasses = annSettings.getNumClasses();

//            // Load images
//            vector<string> test_imgs = Support::pathVector(testDir,".jpg");

//            // Load classifier
//            Classifier * classifier = new myANN();
//            classifier->setLabels(label_array,numClasses);
//            classifier->loadFromFile(netPath.c_str());

//            // get Features from Images
//            cv::Mat_<float> test_features = featureExtractionFromDir(test_imgs,annSettings.vec_features);
//            classifier->setFeatureVectorSize(test_features.cols);
//            //Prediction
//            vector<uchar> predictions = classifier->predict(test_features);
//            vector<uchar> test_labels = classifier->extLabelFromFileName(test_imgs);
//            classifier->evaluate(predictions,test_labels,numClasses);
        }
    }
    else{
        help();
    }
    return 0;
}

