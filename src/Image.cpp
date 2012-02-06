/**
 * @file Image.cpp
 * @brief Image Class
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */

#include "../include/cwpapers/Image.h"
//---------------------------------------------------------------------------
// Constructors
//---------------------------------------------------------------------------
Image::Image(){}
Image::Image(string file,bool inColor){
    filename=file;
    path=file.substr(0,file.rfind("/")+1);
    file=file.substr(file.rfind("/")+1,file.length());
    name=file.substr(0,file.find("."));
    ext=file.substr(file.find(".")+1,file.length());
    //Load the image
    image=imread(filename,inColor);
}
Image::Image(const Image& orig){}
Image::~Image(){}
//---------------------------------------------------------------------------
// Set and Get
//---------------------------------------------------------------------------
Mat Image::getImage(){
    return image;
}
Mat Image::getDescriptors(){
    return descriptors;
}
vector<KeyPoint> Image::getKeypoints(){
    return keypoints;
}
//---------------------------------------------------------------------------
// Methods
//---------------------------------------------------------------------------
void Image::calculate(bool forceCalculate){
    if(forceCalculate){
        calcKeypoints();
        calcDescriptors();
        save();
    }else{
        if(!load()){
            calculate(true);
        }else{
            calcDescriptors();
        }
    }
}
void Image::show(){
    imshow("CWPapers: "+name,image);
    cvWaitKey(0);
}
void Image::calcKeypoints(){
    SiftFeatureDetector detector;
    //keypoints for the image
    detector.detect(image,keypoints);
}
void Image::calcDescriptors(){
    SiftDescriptorExtractor descriptor;
    if(keypoints.size()>0){
        descriptor.compute(image,keypoints,descriptors);
    }else{
        cout<<"Cannot calculate the descriptors of image: "<<name<<endl;
    }
}
void Image::save(){
    string kpPath="data/keypoints/"+name+"Keypoints.txt";
    ofstream file;
    file.open(kpPath.c_str());
    for(int i=0;i<keypoints.size();i++){
        file<<
                keypoints[i].angle      <<" "<<
                keypoints[i].class_id   <<" "<<
                keypoints[i].octave     <<" "<<
                keypoints[i].pt.x       <<" "<<
                keypoints[i].pt.y       <<" "<<
                keypoints[i].response   <<" "<<
                keypoints[i].size       <<endl;
    }
    file.close();
}
bool Image::load(){
    string kpPath="data/keypoints/"+name+"Keypoints.txt";
    vector<KeyPoint> keypointsIn;
    ifstream file;
    string line;
    file.open(kpPath.c_str());
    while(file.good()){
        KeyPoint kp;
        getline(file,line);
        if(line.length()>0){
            kp.angle=atof(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.class_id=atoi(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.octave=atoi(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.pt.x=atof(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.pt.y=atof(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.response=atof(line.substr(0,line.find(" ")).c_str());
            line=line.substr(line.find(" ")+1,line.length());
            kp.size=atof(line.substr(0,line.find(" ")).c_str());
            keypointsIn.push_back(kp);
        }
    }
    file.close();
    keypoints=keypointsIn;
    if(keypoints.size()>0){
        return true;
    }else{
        return false;
    }
}