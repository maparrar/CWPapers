/**
 * @file CWPapers.h
 * @brief Header for class CWPapers
 * @author Alejandro Parra (maparrar@gmail.com) - 2011
 * @details GNU GPL v2 @details Project info http://code.google.com/p/cwpapers/
 */

#ifndef CWPAPERS_H
#define	CWPAPERS_H

#include "Marker.h"
#include <zbar.h>
#include <unistd.h> //detect the path of the app

namespace cwpapers {
    /**
     * Cardinal points used in the markers
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
     */
    enum {NW=0,NE=1,SE=2,SW=3};
    
    class CWPapers {
    public:
    /// Constructors
        CWPapers();
        CWPapers(const CWPapers& orig);
        virtual ~CWPapers();
    /// Set and Get
        Image getInput();
    /// Methods
        /**
         * Main function of CWPapers. Execute the match and the qrread functions
         * @param filename of the image to process
         */
        void process(string filename);
        void addInputImage(string filename);
        void addMarkers(string path,string fileNE,string fileSE,string fileSW);
        /**
         * Find the map in the input image, transform it an save as an independent
         * image only with the map
         * @param cornerQR, if true, use the QR position information to find the NW corner
         *      false, calculate based in the other corners.
         */
        void match(bool cornerQR=false);
        /**
         * Read the QR code in the image and set the variables:
         *  - id:   Identificator of the paper
         *  - lat:  Latitude of the image in the paper
         *  - lon:  Longitude of the image in the paper
         *  - zoom: Level of zoom of the image
         * @return true if the QR code can be read, false in otherwise.
         */
        bool qrread();
    private:
        //Output image
        int outputWidth;
        int outputHeight;
        //Markers and input image
        //TODO: Use a vector
        Marker markerNW;
        Marker markerNE;
        Marker markerSE;
        Marker markerSW;
        Image input;
        //Variables read in the QR code, only are set if qrread returns true.
        Point2f qrPoint;
        int id;
        float lat;
        float lon;
        float zoom;
        //Private methods
//        void loadMarkers();
//        void loadKeypoints();
    };
}       //Close namespace
#endif	/* CWPAPERS_H */