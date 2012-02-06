/**
 * @file Marker.h
 * @brief Header for class Marker
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */
#ifndef MARKER_H
#define	MARKER_H

#include "Image.h"

class Marker:public Image{
public:
    Marker();
    Marker(const Marker& orig);
    virtual ~Marker();
    /**
     * Init the Marker
     * @param cardinal of marker
     *          - 0=NW: Northwest  (Not used)
     *          - 1=NE: Northeast
     *          - 2=SE: Southeast
     *          - 3=SW: Southwest
     *           _______________
     *          |QR  |        NE|
     *          |code|          |
     *          |____|          |
     *          |               |
     *          |               |
     *          |               |
     *          |               |
     *          |               |
     *          |               |
     *          |SW___________SE|
     * 
     * @param file filename name of image marker
     */
    Marker(int cardin,string file);
    /**
     * Get an image to calculate the keypoint's matches. Sets the variable 'point'
     * with the extreme point in the input image from the end of the marker.
     * @param input must be calculated, for get descriptors and keypoints
     */
/// Set and Get
    Point2f getPoint(){return point;};
/// Methods
    void match(vector<KeyPoint> inputKeypoints,Mat inputDescriptors,Mat inputImage);
private:
    int cardinal;
    //Extreme point of Marker, depends of the cardinal
    Point2f point;
};
#endif	/* MARKER_H */