#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;


int main( int argc, char** argv ) {
  
  //CvCapture * cap;
  VideoCapture cap(0);
  //cap = cvCreateCameraCapture(0);
  //while(true){
    Mat save_img;
    cap.read(save_img);  
    
    //Mat hsv_image;
    //cvtColor(save_img, hsv_image, cv::COLOR_BGR2HSV);
    imshow( "Display window", save_img);
    //if(waitKey(30) > 0) break;
  //}
  waitKey(0);
  
  
  
  return 0;
}
