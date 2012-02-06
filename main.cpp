/**
 * @file main.cpp
 * @brief main.cpp for testing the classes
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 * @requirements
 *      - Opencv v.2.1 (http://sourceforge.net/projects/opencvlibrary/)
 *      - zbar v.0.10 (http://zbar.sourceforge.net/)
 * @notes
 *      Compile with includes:
 *              - /usr/local/include/opencv2
 *              - /usr/include/zbar
 *      Link to:
 *              - opencv_calib3d
 *              - opencv_core
 *              - opencv_features2d
 *              - opencv_highgui
 *              - opencv_imgproc
 *              - zbar
 *      The app neds the follow directories under the program directory
 *              /data/images            The images for transform and analyze
 *              /data/keypoints         The program stored the keypoints of the markers
 *              /data/markers           The markers to find in the images
 *              /data/output            The output image transformed
 */
#include <iostream>
#include "include/cwpapers/CWPapers.h"

using namespace std;
using namespace cwpapers;

int main(int argc, char** argv){
    //TODO: Reducir el tamaño de las imágenes que superen los límites, antes de procesarlas
    
    CWPapers papers;
    papers.process(argv[1]);
    return 0;
}