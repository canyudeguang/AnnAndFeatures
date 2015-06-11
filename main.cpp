#include <iostream>

using namespace std;

#include <algorithm>

#include "lib_support/support.h"
#include "lib_support/cvSupport.h"


#include "lib_features/histogramfeatures.h"
#include "lib_features/cornerfeatures.h"
#include "lib_features/edgefeatures.h"
#include "lib_features/experimentfeature.h"

#include "lib_ann/ann.h"

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

        // Define features
        /// Here example with histogram feature only
        HistogramFeatures fHisto; // histogram features
        fHisto.setNumberOfBins(20); // set number of histogram bins

        EdgeFeatures fEdge;
        experimentFeature fExper;


        // put all pointers tp FeatureExtractors objecst into vector
        vector<FeatureExtractor *> vec_extractors;
        vec_extractors.push_back(&fEdge);
        vec_extractors.push_back(&fExper);
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

        /** Label Extraction */
        //defining labels
        uchar EYE_OPEN = 0;
        uchar EYE_CLOSED = 1;
        vector<uchar> labels;
        for(uint i = 0; i < train_images.size(); ++i){
           string std_Label = Support::splitString(train_images[i],'_')[2];
           if(std_Label.find("CLOSE") == 0){
                labels.push_back(EYE_CLOSED);
           }
           else if(std_Label.find("OPEN") == 0){
                labels.push_back(EYE_OPEN);
           }
        }

        /** Ann Training */
        ANN Ann;

        vector<int> layers(2); // 2 layers of neurons,
        fill(layers.begin(), layers.end(), 8); // with 8 neurons in each layer
        Ann.setParameters(Features.cols, layers, 2); // input how many features, layers, output layer classes

        // Check if labels are same size of features
        if(labels.size() == Features.rows){
            //Ann.parametricTrain(Features,labels,20,layers,0); // 20 ieter, 0 hidden layers
            Ann.train(Features,labels,2);
        }
        else{
            cerr << "Labels are not same size as Features:" << endl;
            cerr << labels.size() << " labels, " << Features.rows << " rows" << endl;
        }


        /** ANN Prediction */
        if(argc > 2){
            //Ann.loadFromFile("iter_20_1_hid_20_nodes.yml");
            //Label extraction & Feature extaction
            vector<uchar> test_labels;
            cv::Mat_<float> test_features;

            string test_dir(argv[2]);
            cout << test_dir << endl;
            vector<string> test_imgs = Support::pathVector(test_dir,".jpg");

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

               string std_Label = Support::splitString(test_imgs[i],'_')[2];
               if(std_Label.find("CLOSE") == 0){
                    test_labels.push_back(EYE_CLOSED);
               }
               else if(std_Label.find("OPEN") == 0){
                    test_labels.push_back(EYE_OPEN);
               }
            }

           vector<uchar> predicts = Ann.predict(test_features);
           Ann.evaluate(predicts,test_labels,2);

        }


    }

    return 0;
}

