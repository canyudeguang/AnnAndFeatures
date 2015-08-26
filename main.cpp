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
#include "lib_features/integralfeature.h"
#include "lib_features/brightfeature.h"

#include "lib_features/lbpfeatures.h"

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
        sort(train_images.begin(), train_images.end());

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
        LBPFeatures fLbp;
        HistogramFeatures fhist;
        BrightFeature fBright;

        fLbp.setSize(512,256);

        // put all pointers tp FeatureExtractors objecst into vector
        vector<FeatureExtractor *> vec_extractors;
       // vec_extractors.push_back(&fEdge);
       // vec_extractors.push_back(&fExper);
       // vec_extractors.push_back(&fHisto);
        //vec_extractors.push_back(&fRaw);
       // vec_extractors.push_back(&fHog);
       // vec_extractors.push_back(&fSkelet);
        vec_extractors.push_back(&fBright);

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

            imshow("Image",img);
           // moveWindow("Image",0,0);
           cvSupport::indexBrowser(i,train_images.size());

        }

        for(int i = 0; i <  Features.rows;++i){
            cout << Features.row(i) << ";" <<train_images[i] << endl;
        }

    }
    return 0;
}

