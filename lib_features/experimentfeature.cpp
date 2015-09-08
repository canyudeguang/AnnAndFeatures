#include "experimentfeature.h"


string experimentFeature::name(){
    return "fExper";
}

/** @function thresh_callback */
void experimentFeature::on_trackbar(int event, void* param)
{
    if(param != NULL){
        experimentFeature *ex = (experimentFeature *) param;
        cout << "Thres: " << ex->thres << endl;

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        threshold(ex->img_g, ex->img_tmp, ex->thres, 255, THRESH_BINARY);

        findContours(ex->img_tmp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        for( int i = 0; i < contours.size(); i++ )
             { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
               boundRect[i] = boundingRect( Mat(contours_poly[i]) );
               minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
             }

        cout << " contury " << contours.size() << endl;
        Mat drawing = Mat::zeros( ex->img_tmp.size(), CV_8UC3 );
          for( int i = 0; i< contours.size(); i++ )
             {
               Scalar color = Scalar( ex->rng.uniform(0, 255), ex->rng.uniform(0,255), ex->rng.uniform(0,255) );
               drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
               rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
               circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
             }


       // imshow("xFeature",ex->img_tmp);
      //  imshow("drawing",drawing);
    }
    else{
        cout << param << endl;
    }
}

experimentFeature::experimentFeature(){

}

Mat_<float> experimentFeature::getFeature(Mat &image)
{
    Mat_<float> f;

    this->init_imgs(image);

    //getGradientImage(image);

    //imshow("xFeature", image);

    f = firstSignificantBlop(this->img_g);
    //namedWindow("xFeature", CV_WINDOW_AUTOSIZE);





    //TRACKING

//    this->rng(1357);
//    this->thres = 100;
//    this->max_thres = 255;
//    createTrackbar("Source","xFeature", &this->thres, max_thres, this->on_trackbar, this);
//    this->on_trackbar(0,this);

    return f;
}

Mat_<float> experimentFeature::maxContourWidth(vector<vector<Point> > &contours, int & index){

    int max_width = this->img_orig.cols * 0.9;

    //Mat drawing;
   // this->img_orig.copyTo(drawing);
    Rect min, max, tmp;
    int I = 0;
    for( int i = 0; i < contours.size(); i++ )
    {
        tmp = boundingRect(Mat(contours[i]));
        //rectangle(drawing,tmp,Scalar(255,0,0));
        if(i == 0){
            min = Rect(tmp);
            if(tmp.width < max_width){
                max = Rect(tmp);
            }
        }
        else{
            if( tmp.width > max.width && tmp.width < max_width){
                max = Rect(tmp);
                I = i;
            }

            if( tmp.width < min.width){
                min = Rect(tmp);
            }
        }
    }

    index = I;

    //rectangle(drawing,min,Scalar(0,255,0));
    //rectangle(drawing,max,Scalar(0,0,255));
    //imshow("draginw",drawing);

    Mat_<float> features(1,2);

    if(max.width > img_g.cols * 0.55){
//        cout << max.width << " > " <<  img_g.cols * 0.4 << endl;
//        cout << "max rect: " << max << min << endl;

//        cout << "w/h ratio: " << max.width/(float)max.height << endl;
//        cout << "h/w ratio: " << max.height/(float)max.width << endl;

//        cout << "img height percent " << img_orig.rows/(float)max.height << endl;
//        cout << "img width perc "  << img_orig.cols/(float)max.width << endl;

       // cout << max.height << endl;
       // cout << max.width/(float)max.height << " " << img_orig.rows/(float)max.height << " " << img_orig.cols/(float)max.width << endl;
    }

    features(0,0) = (float)max.width;
    features(0,1) = (float)max.height;
    return features;
}
Mat_<float> experimentFeature::firstSignificantBlop(Mat &img){

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat_<float> fempty(1,2);
    fempty(0,0) = 0;
    fempty(0,1) = 0;

    Mat drawing = Mat::zeros( this->img_tmp.size(), CV_8UC3 );
    for(int thres = 0; thres < 255; thres+=1){

    //cout << " threshold: " << thres << endl;
    // 1. threshold and check contur number
    //=========================================================================
       equalizeHist(this->img_g, this->img_g);
       threshold(this->img_g,this->img_tmp, thres, 255, THRESH_BINARY);
        findContours(this->img_tmp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

        int imaxWidth = 0;
        Mat_<float> maxWidth = this->maxContourWidth(contours,imaxWidth);
        // 2. If there is contour - find the widhest one
        if(contours.size() > 1){

           // int maxWidth = this->maxContourWidth(contours,imaxWidth);

            if( maxWidth(0,0) > img.cols * 0.55){

                drawing = Mat::zeros( this->img_tmp.size(), CV_8UC3 );
                // aprox contur poly
                vector<vector<Point> > contours_poly( contours.size() );
                for( int i = 0; i < contours.size(); i++ ){
                    if( i == imaxWidth){
                        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
                        drawContours(drawing, contours_poly, i, Scalar(0,0,255));
                        //Rect r = boundingRect(Mat(contours_poly[i]));
                        //rectangle(drawing,r, Scalar(0,0,255));
                    }
                }
                //cout << maxWidth << endl;

               // imshow("contur",drawing);
                return maxWidth;
            }

        }

       // waitKey(0);
    }

    //imshow("contur",drawing);
    return fempty;
}

Mat experimentFeature::blops(Mat &img){

    return img;
}

void experimentFeature::init_imgs(Mat &img){
    img.copyTo(this->img_orig);


    cvtColor(this->img_orig, this->img_g, CV_BGR2GRAY);
    cvtColor(this->img_orig, this->img_hsv, CV_BGR2HSV);
}

Mat experimentFeature::haarEye(Mat &img){

    Mat image;
    equalizeHist(this->img_g, image);

    vector<Rect> eyes;
    string file = "../../lib/haars/haarcascade_mcs_righteye.xml";
    CascadeClassifier eyes_cascade;
    if(!eyes_cascade.load(file)){
        cerr << "not loaded " << file << endl;
        return image;
    }

    int w = img.cols;
    int h = img.rows;
    eyes_cascade.detectMultiScale(image, eyes, 1.1, 0, CV_HAAR_DO_ROUGH_SEARCH|CV_HAAR_FEATURE_MAX, Size(1,1));

    /**
      haarcascade_eye_tree_eyeglasses

      0|CV_HAAR_DO_CANNY_PRUNING = 42
      0|CV_HAAR_SCALE_IMAGE = 51
      CV_HAAR_FEATURE_MAX = 51
      CV_HAAR_FIND_BIGGEST_OBJECT = 25
      CV_HAAR_MAGIC_VAL = 42
      CV_HAAR_DO_ROUGH_SEARCH = 42

      */
    static int counter = 0;
    if(eyes.size() >0 )
    {
        cout << "EYES: " << (int)eyes.size() << endl;
        counter++;
    }

    for( size_t i = 0; i< eyes.size(); ++i){
        rectangle(image,eyes[i],Scalar(0,255,0),2);
    }

    cout << (int)counter << " OCI " << endl;
    return image;
}

Mat experimentFeature::otsu(Mat &img){

    Mat img_otsu;

    equalizeHist(this->img_g,this->img_g);
    threshold(this->img_g, img_otsu,0,255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    return img_otsu;
}


Mat experimentFeature::colorReduce(Mat &img){
    equalizeHist(img,img);
    int div = 92;
    int nl = img.rows;                    // number of lines
        int nc = img.cols * img.channels(); // number of elements per line

        for (int j = 0; j < nl; j++)
        {
            // get the address of row j
            uchar* data = img.ptr<uchar>(j);

            for (int i = 0; i < nc; i++)
            {
                // process each pixel
                data[i] = data[i] / div * div + div / 2;
            }
        }

        return img;
}

Mat experimentFeature::getGradientImage(Mat &img){
    // 1. Open Image as GrayScale and RGB and HSV
    //=========================================================================
    this->init_imgs(img);

    vector<Mat> hsv_channels;
    split(this->img_hsv,hsv_channels);

    // 3. Set the Hue channel to Grayscale image
    //=========================================================================

   // equalizeHist(this->img_g, this->img_g);
    convertScaleAbs(this->img_g, this->img_g, 180/256.0); // sclae bg image to degrees

   // imshow("grayqeu", this->img_g);

    this->img_g.copyTo(hsv_channels[0]);


    // 4. Set Value and Saturation both to maximum
    //=========================================================================
    hsv_channels[1].setTo(255);
    hsv_channels[2].setTo(255);

    //adjust pseudo color scaling

    int offset = 120;
    Mat mask1,mask2;
    mask1.create(cv::Size(img.cols, img.rows),CV_8UC1);
    mask2.create(cv::Size(img.cols, img.rows),CV_8UC1);

    cv::compare(hsv_channels[0],180-offset,mask1,CV_CMP_GE);
    cv::compare(hsv_channels[0],180-offset,mask2,CV_CMP_LT);
    cv::add(hsv_channels[0], offset-180, hsv_channels[0],mask1);
    cv::add(hsv_channels[0], offset, hsv_channels[0],mask2);

    cv::merge(hsv_channels,this->img_rgb);
    cvtColor(img_rgb, img_rgb, CV_HSV2RGB);

//    imshow("res", img_rgb);

    // 5. Convert back to RGB space
    //=========================================================================

  //  imshow("orig", this->img_orig);
    return this->img_hsv;
}


