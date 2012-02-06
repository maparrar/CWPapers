/**
 * @file CWPapers.cpp
 * @brief CWPapers Class
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */

#include "../include/cwpapers/CWPapers.h"
using namespace cwpapers;
//---------------------------------------------------------------------------
// Constructors
//---------------------------------------------------------------------------
CWPapers::CWPapers(){
    outputWidth=1200;
    outputHeight=1700;
}
CWPapers::CWPapers(const CWPapers& orig){}
CWPapers::~CWPapers(){}
//---------------------------------------------------------------------------
// Set and Get
//---------------------------------------------------------------------------
Image CWPapers::getInput(){
    return input;
}
//---------------------------------------------------------------------------
// Methods
//---------------------------------------------------------------------------
void CWPapers::process(string filename){
    cout<<"started=true"<<endl;
    cout<<"file="<<filename<<endl;
    addInputImage(filename);  
    addMarkers("data/markers/","markerNE.png","markerSE.png","markerSW.png");
    //Read the QRCode
    if(qrread()){
        match(true);
    }else{
        cout<<"qr=false"<<endl;
        match(false);
    }
}
void CWPapers::addInputImage(string filename){
    char cCurrentPath[FILENAME_MAX];
    getcwd(cCurrentPath, sizeof(cCurrentPath));
    
    string comPath=cCurrentPath;
//    cout<<"  Path ingresada en terminal(appPath): "<<appPath<<endl;
//    cout<<"  Donde ejecuto la app(comPath): "<<comPath<<endl;
    
    input=Image(filename);
}
void CWPapers::addMarkers(string path,string fileNE,string fileSE,string fileSW){
    markerNE=Marker(NE,path+fileNE);
    markerSE=Marker(SE,path+fileSE);
    markerSW=Marker(SW,path+fileSW);
}
void CWPapers::match(bool cornerQR){    
    //Load or calculate the descriptors
    input.calculate(true);
    cout<<"keypoints=true"<<endl;
    //Match each marker with the image
    markerNE.match(input.getKeypoints(),input.getDescriptors(),input.getImage());
    markerSE.match(input.getKeypoints(),input.getDescriptors(),input.getImage());
    markerSW.match(input.getKeypoints(),input.getDescriptors(),input.getImage());
    cout<<"markers=true"<<endl;
    //Get extreme points of each marker to transform the original image and calculate the NW point
    Mat original=input.getImage();
    Point2f pointNE=markerNE.getPoint();
    Point2f pointSE=markerSE.getPoint();
    Point2f pointSW=markerSW.getPoint();
    Point2f pointNW;
    //If the cornerQr=true use the corner of QRcode as NW point, else, is calculated from the others
    if(cornerQR){
        pointNW=qrPoint;
    }else{
        pointNW=Point2f(pointNE.x-pointSE.x+pointSW.x,pointSW.y-pointSE.y+pointNE.y);
    }
    cout<<"NW=["<<pointNW.x<<","<<pointNW.y<<"]"<<endl;
    cout<<"NE=["<<pointNE.x<<","<<pointNE.y<<"]"<<endl;
    cout<<"SE=["<<pointSE.x<<","<<pointSE.y<<"]"<<endl;
    cout<<"SW=["<<pointSW.x<<","<<pointSW.y<<"]"<<endl;
    //Transforms the image between the markers to the output size.
    Point2f pMarkers[4];
    Point2f pOutput[4];
    pMarkers[NW]=pointNW;
    pMarkers[NE]=pointNE;
    pMarkers[SE]=pointSE;
    pMarkers[SW]=pointSW;
    pOutput[NW]=Point2f(0,0);
    pOutput[NE]=Point2f(outputWidth,0);
    pOutput[SE]=Point2f(outputWidth,outputHeight);
    pOutput[SW]=Point2f(0,outputHeight);
    Mat outputImage = Mat(outputHeight,outputWidth,CV_8UC1);
    //Calculate the homography
    Mat H=getPerspectiveTransform(pMarkers,pOutput);
    warpPerspective(original,outputImage,H,outputImage.size(),INTER_LINEAR,BORDER_CONSTANT);
    //Save the output image
    imwrite("data/output/"+input.getName()+"Output.jpg",outputImage);
    cout<<"output=true"<<endl;
    //DEBUG CODE
    bool debug=true;
    if(debug){
        //Draw the trapezoid to enclose the map
        line(original,pointNE,pointSE,CV_RGB(0,0,0));
        line(original,pointSE,pointSW,CV_RGB(0,0,0));
        line(original,pointSW,pointNW,CV_RGB(0,0,0));
        line(original,pointNW,pointNE,CV_RGB(0,0,0));
        imwrite("data/output/"+input.getName()+".jpg",original);
    }
    cout<<"finished=true"<<endl;
}
bool CWPapers::qrread(){
    // create and configure the reader
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    //Read the image in greyscale
    Mat im=imread(input.getFilename(),0);
    int width = im.cols;
    int height = im.rows;
    bool success=false;
    //Get the pointer to image data
    const void *data=im.data;
    //Wrap image data
    zbar::Image image(width, height, "Y800", data, width * height);
    //Scan the image for barcodes
    int n = scanner.scan(image);
    //Extract results
    for(zbar::Image::SymbolIterator symbol=image.symbol_begin();symbol!=image.symbol_end();++symbol){
        
        bool debug=true;
        if(debug){
            //TODO: DECODE THE DATA OF QR CODE
            cout<<"qr="<<symbol->get_data()<<endl;
//            cout<<"get_data_length: "<< symbol->get_data_length()<<endl;
//            cout<<"get_location_size: "<< symbol->get_location_size()<<endl; //Number of points in the location polygon
//            cout<<"Location(0)[x,y]: ["<<symbol->get_location_x(0)<<","<<symbol->get_location_y(0)<<"]"<<endl;
//            cout<<"Location(1)[x,y]: ["<<symbol->get_location_x(1)<<","<<symbol->get_location_y(1)<<"]"<<endl;
//            cout<<"Location(2)[x,y]: ["<<symbol->get_location_x(2)<<","<<symbol->get_location_y(2)<<"]"<<endl;
//            cout<<"Location(3)[x,y]: ["<<symbol->get_location_x(3)<<","<<symbol->get_location_y(3)<<"]"<<endl;
        }
        if(symbol->get_data_length()>0){
            success=true;
            qrPoint.x=symbol->get_location_x(0);
            qrPoint.y=symbol->get_location_y(1);
        }
    }
    //Clean up
    image.set_data(NULL, 0);
    return success;
}
