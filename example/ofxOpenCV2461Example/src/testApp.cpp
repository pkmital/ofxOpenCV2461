#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    width = 640; height = 480;
    
    camera.initGrabber(width, height);
    colorImg.allocate(width, height);
    grayImg.allocate(width, height);
    prevGrayImg.allocate(width, height);
    flowColorImg.allocate(width, height);
    flow = createOptFlow_DualTVL1();
    
    flowImg = Mat(height, width, CV_32FC2);
    
}

//--------------------------------------------------------------
void testApp::update(){
    camera.update();
    
    if(camera.isFrameNew())
    {
        colorImg.setFromPixels(camera.getPixelsRef());
        colorImg.convertRgbToHsv();
        colorImg.convertToGrayscalePlanarImage(grayImg, 2);
        
        Mat I0(grayImg.getCvImage()), I1(prevGrayImg.getCvImage());
        
        flow->calc(I0, I1, flowImg);
        
        //extraxt x and y channels
        Mat xy[2]; //X,Y
        split(flowImg, xy);
        
        //calculate angle and magnitude
        Mat magnitude, angle;
        cartToPolar(xy[0], xy[1], magnitude, angle, true);
        
        //translate magnitude to range [0;1]
        double mag_max;
        minMaxLoc(magnitude, NULL, &mag_max);
        magnitude.convertTo(magnitude, -1, 1.0/mag_max);
        
        //build hsv image
        Mat _hsv[3], hsv;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magnitude;
        merge(_hsv, 3, hsv);
        
        //convert to BGR and show
        Mat rgb, rgb8(flowColorImg.getCvImage());//CV_32FC3 matrix
        cvtColor(hsv, rgb, COLOR_HSV2RGB);
        rgb.convertTo(rgb8, CV_8UC3);
        flowColorImg.flagImageChanged();
        cv::imshow("flow", rgb);
        
        prevGrayImg = grayImg;
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    flowColorImg.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}