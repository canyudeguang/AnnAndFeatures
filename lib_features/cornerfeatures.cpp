#include "cornerfeatures.h"

string CornerFeatures::name(){
    return "fCorner";
}

Mat_<float> CornerFeatures::getFeature(Mat &image)
{
    Mat_<float> Featues(1,1);

    Mat orig, gray;
    image.copyTo(orig);
    cvtColor(orig,gray,CV_BGR2GRAY);

    // Detector parameters
    //==========================================================================================

    /**
      Harris corenrs
      */

    int blockSize = 3;
    int apertureSize = 5;
    double k = 0.14;

    blur(gray,gray,Size(3,3));
    Mat img_corners;
    cornerHarris(gray, img_corners, blockSize, apertureSize, k);
    // Normalize
    Mat dst_norm, dst_norm_scaled;
    normalize( img_corners, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

//    int thresh = 250;
//    int count = 0;
//    /// Drawing a circle around corners
//      for( int j = 0; j < dst_norm.rows ; j++ )
//         { for( int i = 0; i < dst_norm.cols; i++ )
//              {
//                if( (int) dst_norm.at<float>(j,i) > thresh )
//                  {
//                   //circle( image, Point( i, j ), 2 ,  Scalar(0,0,255), 1, 8, 0 );
//                   count++;
//                }
//              }
//         }

//      cout << count << endl;
//    imshow("corners", dst_norm_scaled);
//    imshow("corners1", img_corners);
//    imshow("corners2", dst_norm);
    /**
      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
      Harris corenrs
      */


    /**
      Hough Circle
      */

    vector<Vec3f> circles;
    //GaussianBlur(dst_norm_scaled, dst_norm_scaled, Size(9,9),2,2);

    HoughCircles(dst_norm_scaled, circles, CV_HOUGH_GRADIENT,1, gray.rows/8, 200, 10, 1,30);

    //cout << "Found: " << circles.size() << endl;
    for( size_t i = 0; i < circles.size(); i++ )
      {
          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          int radius = cvRound(circles[i][2]);
          // circle center
          circle( orig, center, 3, Scalar(0,255,0), -1, 8, 0 );
          // circle outline
          circle( orig, center, radius, Scalar(0,0,255), 2, 8, 0 );
       }

    imshow( "Hough Circle Transform Demo", orig );
    imshow( "Hough Circle Transform Demoasda", dst_norm_scaled );

    Featues(0,1) = circles.size();
    return Featues;
}

