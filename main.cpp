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

        cout << "From: " << directory << " "<< train_images.size() << " images loaded." << endl;
        // Define features
        /// Here example with histogram feature only
        HistogramFeatures fHisto; // histogram features
        fHisto.setNumberOfBins(4); // set number of histogram bins

        EdgeFeatures fEdge;
        experimentFeature fExper;
        RawFeatures fRaw;
        SkeletFeatures fSkelet;
        HOGFeatures fHog;

        // put all pointers tp FeatureExtractors objecst into vector
        vector<FeatureExtractor *> vec_extractors;
       // vec_extractors.push_back(&fEdge);
        vec_extractors.push_back(&fExper);
       // vec_extractors.push_back(&fHisto);
       // vec_extractors.push_back(&fRaw);
        vec_extractors.push_back(&fHog);
       // vec_extractors.push_back(&fSkelet);

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




        /** Ann Training */

        ANN Ann;

        // set labels
        int numClasses = 4;
        string str_labels[numClasses] = {"OPEN","DEFAULT","TONGUE","TEETH"};
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
        fill(layers.begin(), layers.end(), 4); // with 8 neurons in each layer
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

        /** ANN Prediction */

        if(argc > 2){
            //Ann.loadFromFile("iter_20_1_hid_20_nodes.yml");
            //Label extraction & Feature extaction

            cv::Mat_<float> test_features;
            string test_dir(argv[2]);

            cout << "ANN predict ... from " << test_dir << endl;

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
            }

            vector<uchar> test_labels = Ann.extLabelFromFileName(test_imgs);
            vector<uchar> predicts = Ann.predict(test_features);
            Ann.evaluate(predicts,test_labels,numClasses);

        }
    }
    return 0;
}

