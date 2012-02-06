/**
 * @file Image.h
 * @brief Header for class Image
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */
#ifndef IMAGE_H
#define	IMAGE_H

#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
/**
 * @class Image
 * Contains an image/array, load an image, calc keypoints and descriptors
 */
class Image{
public:
/// Constructors
    Image();
    /**
     * Constructor
     * @param file filename of the image
     */
    Image(string file,bool inColor=true);
    Image(const Image& orig);
    virtual ~Image();
/// Set and Get
    string getFilename(){return filename;};
    string getPath(){return path;};
    string getName(){return name;};
    string getExt(){return ext;};
    Mat getImage();
    Mat getDescriptors();
    vector<KeyPoint> getKeypoints();
/// Methods
    void show();
    /**
     * Calculate arrays and vectors from the image. Calculates:
     *  - keypoints: To calculate descriptors of image
     *  - descriptors: To match to another images
     * @param forceCalculate force the arrays an vectors calculation
     *    if forceCalculate=false
     *       - Try to load the Mats from file
     *          - if false calculates keypoints and descriptors
     *          - else load Mats from file
     *    if forceCalculate=true
     *       - Calculates the Mats and overwrite the files
     */
    void calculate(bool forceCalculate=false);
    
protected:
    string filename;
    string path;
    string name;
    string ext;
    Mat image;
    vector<KeyPoint> keypoints;
    Mat descriptors;    
    void calcKeypoints();
    void calcDescriptors();
    void save();
    bool load();
};
#endif	/* IMAGE_H */