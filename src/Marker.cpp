/**
 * @file Marker.cpp
 * @brief Marker Class
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */

#include "../include/cwpapers/Marker.h"

Marker::Marker() {}
Marker::Marker(const Marker& orig){}
Marker::~Marker(){}
Marker::Marker(int cardin,string file){
    cardinal=cardin;
    filename=file;
    path=file.substr(0,file.rfind("/")+1);
    file=file.substr(file.rfind("/")+1,file.length());
    name=file.substr(0,file.find("."));
    ext=file.substr(file.find(".")+1,file.length());
    //Load the image in greyscale
    image=imread(filename,false);
}

void Marker::match(vector<KeyPoint> inputKeypoints,Mat inputDescriptors,Mat inputImage){
    BruteForceMatcher<L2<float> > matcher;
    vector<DMatch> matches;
    //Calculate the descriptors of the marker
    calculate();
    //Match the marker with the input image
    matcher.match(descriptors,inputDescriptors,matches);
    //Extract pairs of points
    vector<int> pairMarkerKPs(matches.size()), pairInputKPs(matches.size());
    for( size_t i = 0; i < matches.size(); i++ ){
        pairMarkerKPs[i] = matches[i].queryIdx;
        pairInputKPs[i] = matches[i].trainIdx;
    }
    //Converts the keypoints to Point2f vectors
    vector<Point2f> markerPoints;
    vector<Point2f> inputPoints;
    KeyPoint::convert(keypoints,markerPoints,pairMarkerKPs);
    KeyPoint::convert(inputKeypoints,inputPoints,pairInputKPs);
    //Matched pairs of 2D points. Those pairs will be used to calculate homography
    Mat marker2Dfeatures;
    Mat input2Dfeatures;
    Mat(markerPoints).copyTo(marker2Dfeatures);
    Mat(inputPoints).copyTo(input2Dfeatures);
    //Calculates the homography
    vector<uchar> outlierMask;
    Mat H;
    H = findHomography(marker2Dfeatures,input2Dfeatures,outlierMask,RANSAC,3);
    //Create the mask to calc the extreme points
    int width=image.size().width;
    int height=image.size().height;
    Mat mask = Mat(height,width,CV_8UC1,0.0);
    //Draw a white point in the mask to detect the marker in the input image
    switch(cardinal){
        case 0:         //NW
            mask.at<uchar>(height-1,0)=255;
        break;
        case 1:         //NE
            mask.at<uchar>(height-1,width-1)=255;
        break;
        case 2:         //SE
            mask.at<uchar>(0,width-1)=255;
        break;
        case 3:         //SW
            mask.at<uchar>(0,0)=255;
        break;
    }
    //Detect the mask in the inputImage image using the homography
    Mat maskedImage;
    warpPerspective(mask,maskedImage,H,inputImage.size(),INTER_LINEAR,BORDER_CONSTANT);
    //FIXIT: Get the pixel with maximum value
    //Find the point in the maskedImage
    Point2f extremePoint;
    for(int i=0;i<maskedImage.rows;i++){
        for(int j=0;j<maskedImage.cols;j++){
            int pixel= maskedImage.at<uchar>(i,j);
            if(pixel>0){
                extremePoint.x=j;
                extremePoint.y=i;
                i=maskedImage.rows;
                j=maskedImage.cols;
            }
        }
    }
    //Returns the point
    point=extremePoint; 
    //DEBUG CODE
    bool debug=false;
    if(debug){
        //Shows the match between keypoints marker and keypoints input image
        Mat outimg;
        drawMatches(image,keypoints,inputImage,inputKeypoints,matches,outimg, Scalar::all(-1), Scalar::all(-1),reinterpret_cast<const vector<char>&> (outlierMask));
        imwrite("data/output/"+name+"_keypoints.jpg",outimg);
        
        //Draws a cross in the extreme point
        Mat cross=inputImage;    
        line(cross,Point(point.x,0),Point(point.x,cross.rows),CV_RGB(200,200,200));
        line(cross,Point(0,point.y),Point(cross.cols,point.y),CV_RGB(200,200,200));
        imwrite("data/output/"+name+"_corners.jpg",cross);

        //Save a text file with the matches between the marker and the input image
        ofstream km;
        string nameMatches="data/output/"+name+"_matches.tx";
        km.open(nameMatches.c_str());
        km<<
                "markerId(Qrx,Qry)\t"
                "inputId(Scanx,Scany)\t"
                "distance"
                "imgIdx"<<endl;
        for(int i=0;i<matches.size();i++){
            km<<
                matches[i].queryIdx<<"("<<keypoints[matches[i].queryIdx].pt.x<<","<<keypoints[matches[i].queryIdx].pt.y<<")\t"<<
                matches[i].trainIdx<<"("<<inputKeypoints[matches[i].trainIdx].pt.x<<","<<inputKeypoints[matches[i].trainIdx].pt.y<<")\t"<<
                matches[i].distance<<"\t"<<
                matches[i].imgIdx<<endl;
        }
        km.close();
    }
}