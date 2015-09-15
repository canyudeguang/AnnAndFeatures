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

void printFeaturesConfig(vector<FeatureExtractor *> & vec_extractors){
    cout << "FEATURES: ";
    for(int k = 0; k < vec_extractors.size(); ++k){
        cout << vec_extractors[k]->name();
        if(k != vec_extractors.size()-1) cout << ", ";
    }
    cout << endl;
}


void TestAllConfig(vector<string> & train_images, string directory, string test_dir){
    HistogramFeatures fHisto; // histogram features
    fHisto.setNumberOfBins(256); // set number of histogram bins
    EdgeFeatures fEdge;
    experimentFeature fExper;
    RawFeatures fRaw;
    SkeletFeatures fSkelet;
    HOGFeatures fHog;
    BrightFeature fBright;
    LBPFeatures fLbp;

    //FeatureExtractor *allArray[] = {&fExper, &fEdge, &fHisto, &fRaw, &fHog, &fLbp, &fBright, &fSkelet};
    // put all pointers tp FeatureExtractors objecst into vector
    vector<FeatureExtractor *> allPointers;
    allPointers.push_back(&fExper);
    allPointers.push_back(&fEdge);
    allPointers.push_back(&fHisto);
    //allPointers.push_back(&fRaw);
    allPointers.push_back(&fHog);
    allPointers.push_back(&fLbp);
    allPointers.push_back(&fBright);
    allPointers.push_back(&fSkelet);
    vector<FeatureExtractor *> vec_extractors;

    FeaturesPicker fpick;
    vector<vector<int> > Indexes = fpick.powerSetIndexes(allPointers.size());

    cout << "Training from " << directory << endl;
    cout << "Predict ... from " << test_dir << endl;


            for(int X = 0; X < Indexes.size(); ++X){
                vector<int> index = Indexes[X];
                if(index.size() > 0){
                    for(int I = 0; I < index.size(); ++I){
                        vec_extractors.push_back(allPointers[index[I]]);
                    }

                printFeaturesConfig(vec_extractors);

               /// CODE GOES HERE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
               /** Feature extraction */
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
               Classifier * classifier = new myANN();


               // set labels
               static const int numClasses = 2;
               string str_labels[numClasses] = {"DEFAULT","REST"};

               classifier->setLabels(str_labels,numClasses);

               // Lebel extraction
               vector<uchar> eLabels = classifier->extLabelFromFileName(train_images);

               if(classifier->hasNullLabel()){
                   cerr << "Unknown label found" << endl;
                   cerr << "Training without UNKNOWNS" << endl;
               }
               if(eLabels.size() == Features.rows){
                   classifier->train(Features, eLabels, numClasses);
                 //  classifier->save2file(string("ann_"+std::to_string(X)).c_str());
               }
               /** SVM Prediction */
               cv::Mat_<float> test_features;



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
               //classifier->evaluate(predictions,test_labels,numClasses) ;
               cout << classifier->evaluateVerbose(predictions,test_labels,numClasses) << "%" << endl;
               /// CODE ENDS HERE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
               vec_extractors.clear();
               delete classifier;
               }// End if index.size >0
            }// end Indexes
}






/**
 * In this example we are using images of Eyes as the training set
 * example of file: TRAIN/eyes/00001_RE_CLOSED.jpg
 * Label is written in the file name "CLOSED"
 * FeatureVector is extracted from the image
 */
int mainOld(int argc, char ** argv)
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

        vector<FeatureExtractor *> vec_extractors;
       // vec_extractors.push_back(&fExper);
       // vec_extractors.push_back(&fEdge);
       // vec_extractors.push_back(&fHisto);
       // vec_extractors.push_back(&fRaw);
        vec_extractors.push_back(&fHog);
       // vec_extractors.push_back(&fLbp);
       // vec_extractors.push_back(&fBright);
       // vec_extractors.push_back(&fSkelet);


        if(argc == 4){
            string argument(argv[3]);
            if(argument == "-all"){
                TestAllConfig(train_images,directory,argv[2]);
                exit(0);
            }
        }

        printFeaturesConfig(vec_extractors);
        /*
         * Open each image from training set
         * extract features
         * extract label
         * save them into final feature vector
         */
        /** Feature extraction */
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
        Classifier * classifier = new myANN();
        cout << "Training from " << directory << endl;

        // set labels
        static const int numClasses = 2;
        string str_labels[numClasses] = {"DEFAULT","REST"};

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
            classifier->save2file("SVM_D_R.yml");
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


        delete classifier;

//
//            }
//        }

        /** SVM Prediction from File*/

//        Classifier *clasFile = new ANN();
//        clasFile->loadFromFile("ANN_D_R.yml");
//        // set labels
//        clasFile->setLabels(str_labels,numClasses);


//        vector<uchar> predictions = clasFile->predict(test_features);
//        vector<uchar> test_labels = clasFile->extLabelFromFileName(test_imgs);
//        clasFile->evaluate(predictions,test_labels,numClasses);


    }

  return 0;
}
