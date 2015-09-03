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

#include "lib_features/featurespicker.h"

#include "lib_classifiers/svm.h"
#include "lib_classifiers/ann.h"
#include "lib_classifiers/boostclass.h"
#include "lib_classifiers/kn.h"
#include "lib_classifiers/decisiontree.h"

/**
 * In this example we are using images of Eyes as the training set
 * example of file: TRAIN/eyes/00001_RE_CLOSED.jpg
 * Label is written in the file name "CLOSED"
 * FeatureVector is extracted from the image
 */
int main(int argc, char ** argv)
{
    if(argc > 1){
        string directory(argv[1]);

        // load all images into vector
        vector<string> train_images = Support::pathVector(directory,".jpg");

        cout << "From: " << directory << " "<< train_images.size() << " images loaded." << endl;
        // Define features
        /// Here example with histogram feature only
        HistogramFeatures fHisto; // histogram features
        fHisto.setNumberOfBins(256); // set number of histogram bins

        EdgeFeatures fEdge;
        experimentFeature fExper;
        RawFeatures fRaw;
        SkeletFeatures fSkelet;
        HOGFeatures fHog;
        BrightFeature fBright;
        LBPFeatures fLbp;

        // put all pointers tp FeatureExtractors objecst into vector
        vector<FeatureExtractor *> allPointers;
        allPointers.push_back(&fExper);
        allPointers.push_back(&fEdge);
        allPointers.push_back(&fHisto);
        allPointers.push_back(&fRaw);
        allPointers.push_back(&fHog);
        allPointers.push_back(&fLbp);
        allPointers.push_back(&fBright);
        allPointers.push_back(&fSkelet);

        vector<FeatureExtractor *> vec_extractors;


        FeaturesPicker fpick;
        vector<vector<int> > Indexes = fpick.powerSetIndexes(allPointers.size());

        for(int X = 0; X < Indexes.size(); ++X){
            vector<int> index = Indexes[X];
            if(index.size() > 0){
                for(int I = 0; I < index.size(); ++I){
                    vec_extractors.push_back(allPointers[index[I]]);
                }

                cout << "FEATURES: " << endl;
                for(int k = 0; k < vec_extractors.size(); ++k){
                    cout << vec_extractors[k]->name();
                    if(k != vec_extractors.size()-1) cout << ", ";
                }
                cout << endl << endl;
                /// #######################################################
                /// Here all codes Start
                /** Feature Extraction */
                        /*
                         * Open each image from training set
                         * extract features
                         * extract label
                         * save them into final feature vector
                         */

                        cv::Mat_<float> Features; // Mat of feature vectors, each row is featureVec from one image

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
                        /** SVM Training */

                        Classifier * classifier = new ANN();

                        cout << "Training from " << directory << endl;

                        // set labels
                        static const int numClasses = 2;
                        string str_labels[numClasses] = {"OPEN","REST"};

                        classifier->setLabels(str_labels,numClasses);

                        // Lebel extraction
                        vector<uchar> eLabels = classifier->extLabelFromFileName(train_images);


                        if(classifier->hasNullLabel()){
                            cerr << "Unknown label found" << endl;
                            cerr << "Training without UNKNOWNS" << endl;
                        }

                //        for(int i = 0; i < eLabels.size();++i){
                //            cout << train_images[i] << "\t" << svm.getLabelString(eLabels[i]) << ":\t" << (int)eLabels[i] << endl;
                //        }

                        if(eLabels.size() == Features.rows){
                            classifier->train(Features, eLabels, numClasses);
                            classifier->save2file("ANN_S_R.yml");
                        }
                        /** SVM Prediction */
                       cv::Mat_<float> test_features;
                       string test_dir(argv[2]);

                       cout << "Predict ... from " << test_dir << endl;

                       vector<string> test_imgs = Support::pathVector(test_dir,".jpg");
                       sort(test_imgs.begin(), test_imgs.end());

                       for(uint i = 0; i < test_imgs.size(); ++i){
                           cv::Mat img = imread(test_imgs[i],CV_LOAD_IMAGE_ANYCOLOR);
                          // joined features from all extractors
                          Mat_<float> fjoined;
                          fjoined.setTo(0);

                          for(uint j = 0; j < vec_extractors.size(); ++j){
                              Mat_<float> f = vec_extractors[j]->getFeature(img);
                              FeatureExtractor::joinFeatures(fjoined,f);
                          }
                          test_features.push_back(fjoined); // add feature_vector to mat of all features
                       }

                       vector<uchar> predictions = classifier->predict(test_features);
                       vector<uchar> test_labels = classifier->extLabelFromFileName(test_imgs);
                       classifier->evaluate(predictions,test_labels,numClasses);


                /// All codes ends
                /// #######################################################
                vec_extractors.clear();
            }
        }

        /** SVM Prediction from File*/

//        Classifier *clasFile = new ANN();
//        clasFile->loadFromFile("ANN_D_R.yml");
//        // set labels
//        clasFile->setLabels(str_labels,numClasses);


//        vector<uchar> predictions = clasFile->predict(test_features);
//        vector<uchar> test_labels = clasFile->extLabelFromFileName(test_imgs);
//        clasFile->evaluate(predictions,test_labels,numClasses);


    }

        /*
        Ann.setLabels(str_labels,numClasses);

        // label extraction
        vector<uchar> eLabels = Ann.extLabelFromFileName(train_images);


        if(Ann.hasNullLabel()){
            cerr << "Error in Label extraction" << endl;
            for(int i = 0; i < eLabels.size() ; ++i){
                cout << train_images[i] <<" " << Ann.getLabelString(eLabels[i]) << " : " << (int)eLabels[i] << endl;
            }
        }

        // Hidden layer if data are not linearly separable
        // Most of the problems are solved by 1 hidden layer, deeper hidden layers are too small differences
        // How many neurons? usually between size of input and size of output
        vector<int> layers(0); // 2 layers of neurons,
        fill(layers.begin(), layers.end(), 2); // with 8 neurons in each layer
        Ann.setParameters(Features.cols, layers, numClasses); // input how many features, layers, output layer classes

        cout << "ANN training ... " << endl;

        // Check if labels are same size of features
        if(eLabels.size() == Features.rows){
           Ann.parametricTrain(Features,eLabels,100, layers, 0); // 20 ieter, 0 hidden layers
           //Ann.train(Features,eLabels,2);
        }
        else{
            cerr << "Labels are not same size as Features:" << endl;
            cerr << eLabels.size() << " labels, " << Features.rows << " rows" << endl;
        }
    */

        /** ANN Prediction */

        /*
        if(argc > 2){
            //Ann.loadFromFile("iter_20_1_hid_20_nodes.yml");
            //Label extraction & Feature extaction

            cv::Mat_<float> test_features;
            string test_dir(argv[2]);

            cout << "ANN predict ... from " << test_dir << endl;

            vector<string> test_imgs = Support::pathVector(test_dir,".jpg");
            sort(test_imgs.begin(), test_imgs.end());

            for(uint i = 0; i < test_imgs.size(); ++i){
                cv::Mat img = imread(test_imgs[i],CV_LOAD_IMAGE_ANYCOLOR);
               // joined features from all extractors
               Mat_<float> fjoined;
               fjoined.setTo(0);

               for(uint j = 0; j < vec_extractors.size(); ++j){
                   Mat_<float> f = vec_extractors[j]->getFeature(img);
                   FeatureExtractor::joinFeatures(fjoined,f);
               }
               test_features.push_back(fjoined); // add feature_vector to mat of all features
            }




            vector<uchar> test_labels = Ann.extLabelFromFileName(test_imgs);
            vector<uchar> predicts = Ann.predict(test_features);
            Ann.evaluate(predicts,test_labels,numClasses);


        }
    }\
    */
    return 0;
}

