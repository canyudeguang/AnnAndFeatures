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
#include "lib_features/maskfeatures.h"
#include "lib_features/brightfeature.h"
#include "lib_features/pointyfeature.h"
#include "lib_features/siftfeatures.h"
#include "lib_features/surffeatures.h"
#include "lib_features/fastfeatures.h"
#include "lib_features/geffeatures.h"
#include "lib_features/testfeature.h"

#include "lib_features/lbpfeatures.h"

#include "lib_detector/detector.h"
#include "lib_features/facedescription.h"

#include "lib_ann/ann.h"

#include "lib_stasm/stasm.h"

cv::Point get_point_landmark(int index, float * landmarks){
    return cv::Point(landmarks[index*2],landmarks[index*2+1]);
}


FaceState getStasmPoints(float * landmarks){
    DoublePoint mouth(get_point_landmark(L_LMouthCorner,landmarks), get_point_landmark(L_RMouthCorner,landmarks));
    DoublePoint lEye(get_point_landmark(L_LEyeOuter,landmarks), get_point_landmark(L_LEyeInner,landmarks));
    DoublePoint rEye(get_point_landmark(L_REyeInner,landmarks), get_point_landmark(L_REyeOuter,landmarks));

    FaceState newFs(Eye(lEye.getLPoint(),lEye.getRPoint()),
                    Eye(rEye.getLPoint(),rEye.getRPoint()),
                    MouthSimple(mouth.getLPoint(), mouth.getRPoint()));
    return newFs;
}

void showStasm(float *stasmLandmarks, string name, Mat & img){
        Mat model;
        img.copyTo(model);

        // Jaw
        for(int i = 0; i < 12; ++i){
            circle(model,Point(get_point_landmark(i,stasmLandmarks)),1,cv::Scalar(0,100,255),2);
        }
        // Forehead
        for(int i = 12; i < 16; ++i){
            circle(model,Point(get_point_landmark(i, stasmLandmarks)),1,cv::Scalar(0,0,255),2);
        }
        // EyeBrows
        for(int i = 16; i < 28; ++i){
            circle(model,Point(get_point_landmark(i, stasmLandmarks)),1,cv::Scalar(200,0,255),2);
        }
        // Eyes
        for(int i = 28; i < 48; ++i){
            circle(model,Point(get_point_landmark(i, stasmLandmarks)),1,cv::Scalar(255,128,0),2);
        }
        // Nose
        for(int i = 48; i < 59; ++i){
            circle(model,Point(get_point_landmark(i, stasmLandmarks)),1,cv::Scalar(50,255,0),2);
        }
        // Mouth
        for(int i = 59; i < stasm_NLANDMARKS; ++i){
            circle(model,Point(get_point_landmark(i, stasmLandmarks)),1,cv::Scalar(0,50,255),2);
        }



    imshow(name,model);
}
vector<cv::Point> performStasm(cv::Mat & image){
    vector<cv::Point> vecLandmarks;
    // convert to grayscale
    cv::Mat_<unsigned char> img_gray;
    cvtColor(image,img_gray,CV_BGR2GRAY);

    int foundface;

    float stasmLandmarks[2*stasm_NLANDMARKS]; // x, y coordinates of stasm points

    if(!stasm_search_single(&foundface, stasmLandmarks, (char*)img_gray.data, img_gray.cols, img_gray.rows, "", "./lib_stasm/haars/")){
        cerr << "Error in stasm_search_single: " << stasm_lasterr() << endl;
    }
    if(foundface > 0){
        //FaceState fs = getStasmPoints(stasmLandmarks);
        for(int i = 0; i < 2*stasm_NLANDMARKS-1; i+=2){
            vecLandmarks.push_back(cv::Point(stasmLandmarks[i],stasmLandmarks[i+1]));
        }
    }
    else{
        cerr << "No faces found" << endl;
        cout << "Face NOT found: ";
    }
    return vecLandmarks;
}

/**
 * In this example we are using images of Eyes as the training set
 * example of file: TRAIN/face/00001.jpg
 * and labels --||--.txt
 * Image is required to be rotated and aligned with face.
 */
int main(int argc, char ** argv)
{
    if(argc > 1){
        string directory(argv[1]);

        // load all images into vector
        vector<string> train_images = Support::pathVector(directory,".jpg");
        sort(train_images.begin(), train_images.end());

        vector<string> train_labels = Support::pathVector(directory,".txt");
        sort(train_labels.begin(), train_labels.end());

        cout << "From: " << directory << " "<< train_images.size() << " images loaded." << endl;
        cout <<  "with: " << train_labels.size() << " labels" << endl;

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
        maskfeatures fMask;
        IntegralFeature fInte;

        testFeature fTest;


        PointyFeature fPoint;

        fLbp.setSize(512,256);

        SiftFeatures fSift;
        SurfFeatures fSurf;
        FastFeatures fFast;
        GEFfeatures fGef;
        // put all pointers tp FeatureExtractors objecst into vector
        vector<FeatureExtractor *> vec_extractors;
       // vec_extractors.push_back(&fEdge);
       // vec_extractors.push_back(&fExper);
       // vec_extractors.push_back(&fHisto);
        //vec_extractors.push_back(&fRaw);
      // vec_extractors.push_back(&fHog);
        //vec_extractors.push_back(&fBright);
        vec_extractors.push_back(&fTest);
        /** Feature Extraction */
        /*
         * Open each image from training set
         * extract features
         * extract label
         * save them into final feature vector
         */


         FaceDetector * fd = new FaceDetector();
        cv::Mat_<float> Features; // Mat of feature vectors, each row is featureVec from one image

        for(uint i = 0; i < train_images.size(); ++i){
            cv::Mat img = imread(train_images[i],CV_LOAD_IMAGE_ANYCOLOR);

            // joined features from all extractors
            Mat_<float> fjoined;
            fjoined.setTo(0);

            for(uint j = 0; j < vec_extractors.size(); ++j){
                Mat_<float> f;
                // is instance of? PointyFeature
                if( PointyFeature * p = dynamic_cast<PointyFeature *>(vec_extractors[j])){
                    //Detect face;
                    vector<cv::Point> vec_landmarks = performStasm(img);
                    PointyFeature * pf = (PointyFeature *) vec_extractors[j];
                    pf->setPoints(vec_landmarks);
                    f = pf->getFeature(img);
                }
                else{
                   f = vec_extractors[j]->getFeature(img);
                }

                FeatureExtractor::joinFeatures(fjoined,f);
            }
            Features.push_back(fjoined); // add feature_vector to mat of all features

           // FaceState fs;
            //fs.openFromFile(train_labels[i]);q

            cout << Features.row(i) <<  " " << train_images[i] << endl;


            imshow("Image",img);
            moveWindow("Image",0,0);
            cvSupport::indexBrowser(i,train_images.size());

        }


    }
    return 0;
}

