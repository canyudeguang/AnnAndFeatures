#include "svm.h"

mySVM::mySVM()
{

}

/*
 * Non linearabe separated data - Separating hyperplane
 *
 *
 */
void mySVM::train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses){
    CvSVMParams params;
    params.svm_type = CvSVM::NU_SVC;
    params.nu = 0.2;
    params.kernel_type = CvSVM::POLY;
    params.gamma = 25;
    params.degree = 4;
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,1000000,0.000001);

#if INFO
    cout << "SVM training ..." << endl;
#endif
    int numberOfSamples = trainData.rows;
    cv::Mat trainLabels = Classifier::cr8ResponseMat(labels, numberOfSamples);
    cvsvm = new CvSVM();
    cvsvm->train(trainData,trainLabels,cv::Mat(),cv::Mat(),params);
#if INFO
    cout << "SVM training ready with sv_count: " << cvsvm->get_support_vector_count() <<" vars: " << cvsvm->get_var_count() << endl;
#endif
}

std::vector<uchar> mySVM::predict(cv::Mat_<float> &testData){
    std::vector<uchar> responses;

    for(int i = 0; i < testData.rows; ++i){
        float response = cvsvm->predict(testData.row(i));
        responses.push_back(response);
#if DEBUG
        cout << testData.row(i) << ": " <<response << endl;
#endif
    }
    return responses;
}

uchar mySVM::predictResponse(cv::Mat_<float> &testData){
    return 0;
}

void mySVM::setTestData(cv::Mat_<float> &testData, vector<uchar> &labels){
    testData.copyTo(mTestData);
    for(int i = 0; i < labels.size();++i){
        this->mLabels.push_back(labels[i]);
    }

}

void mySVM::showGraph(int featuresNum){
    // Data for visual representation
    int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    Vec3b green(0,255,0), blue (255,0,0), white(255,255,255);
        // Show the decision regions given by the SVM
        for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j)
            {
                Mat sampleMat = (Mat_<float>(1,2) << j,i);
                float response = cvsvm->predict(sampleMat);

                if (response == 1)
                    image.at<Vec3b>(i,j)  = green;
                else if (response == 0)
                    image.at<Vec3b>(i,j)  = blue;
                else if (response == 2)
                    image.at<Vec3b>(i,j)  = white;
            }

        // Show the training data
        if(!this->mTestData.empty()){
            int thickness = -1;
            int lineType = 8;
            for(int tdi = 0; tdi < this->mTestData.rows; ++tdi){
                if(mLabels[tdi] == 0){
                     circle(image, Point(mTestData.row(tdi)), 2, Scalar(0,0,255),thickness,lineType);
                }
                else if(mLabels[tdi] == 1){
                      circle(image, Point(mTestData.row(tdi)), 2, Scalar(0,0,0),thickness,lineType);
                }
                else if (mLabels[tdi] == 2){
                       circle(image, Point(mTestData.row(tdi)), 2, Scalar(128,128,0),thickness,lineType);
                }

            }
        }


//        // Show support vectors
//        int thickness = 2;
//        int lineType  = 8;
//        int c     = cvsvm->get_support_vector_count();

//        for (int i = 0; i < c; ++i)
//        {
//            const float* v = cvsvm->get_support_vector(i);
//            cout <<Point( (int) v[0], (int) v[1]) << endl;
//            circle( image,  Point( (int) v[0], (int) v[1]),6,  Scalar(128, 128, 128), thickness, lineType);
//        }

       // imwrite("result.png", image);        // save the image

        imshow("SVM Simple Example", image); // show it to the user
        waitKey(0);
}
