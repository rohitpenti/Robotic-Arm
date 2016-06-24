#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
int main( int argc, char** argv )
 {
 	 
                                                    ///////////////////////////////////////////////////////////////
                                                    ///////////////////////////////////////////////////////////////
                                                    /////////*******************************************///////////
                                                    /////////**************1 FOR YELLOW*****************///////////
                                                    /////////************HUE VALUES: 22 - 38************///////////
                                                    ///////////////////////////////////////////////////////////////
                                                    ///////////////////////////////////////////////////////////////
                                                    /////////*******************************************///////////
                                                    /////////**************2 FOR RED********************///////////
                                                    /////////************HUE VALUES: 160 - 179**********///////////
                                                    ///////////////////////////////////////////////////////////////
                                                    ///////////////////////////////////////////////////////////////


   int LowH1 = 38;
 int HighH1 = 130;             int count1=0,count2=0;                     
                                                    
  int LowS1 = 80;                                    
 int HighS1 = 255;                                 
                                                    
  int LowV1 = 0;                                    
  int HighV1 = 255;                                  
                                                    
  int LowH2 = 160;                                    
 int HighH2 = 179;                                  
                                                    
 int LowS2 = 0;                                    
 int HighS2 = 255;

  int LowV2 = 0;
 int HighV2 = 255;
 namedWindow("Control1", CV_WINDOW_AUTOSIZE); //create a window called "Control"
 	namedWindow("Control2", CV_WINDOW_AUTOSIZE);
 
  //Create trackbars in "Control" window
 createTrackbar("LowH", "Control1", &LowH1, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control1", &HighH1, 179);

  createTrackbar("LowS", "Control1", &LowS1, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control1", &HighS1, 255);

  createTrackbar("LowV", "Control1", &LowV1, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control1", &HighV1, 255);

 createTrackbar("LowH", "Control2", &LowH2, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control2", &HighH2, 179);

  createTrackbar("LowS", "Control2", &LowS2, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control2", &HighS2, 255);

  createTrackbar("LowV", "Control2", &LowV2, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control2", &HighV2, 255);

 while(1)
 {
 	
 	VideoCapture c(0);
 	Mat original;	
 	c.read(original); 
 	
 	  Mat imgThresholded1; Mat imgHSV1;
  Mat imgThresholded2; Mat imgHSV2;
 	   cvtColor(original, imgHSV1, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 		cvtColor(original, imgHSV2, COLOR_BGR2HSV);

   inRange(imgHSV1, Scalar(LowH1, LowS1, LowV1), Scalar(HighH1, HighS1, HighV1), imgThresholded1); //Threshold the image
       inRange(imgHSV2, Scalar(LowH2, LowS2, LowV2), Scalar(HighH2, HighS2, HighV2), imgThresholded2);
  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological opening (removes small objects from the foreground)
  erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  
  	imshow("1",imgThresholded1);imshow("2",imgThresholded2);
  	imshow("orig.",original);
  // 	char k=waitKey(30);
  // 	if (k==27)
  // 	{
  // 		/* code */
  // 	}
   		vector<vector<Point> > contours2; 		
		 vector<Vec4i> hierarchy2;	
		 vector<vector<Point> > contours1; 		
		 vector<Vec4i> hierarchy1;
		 findContours(imgThresholded1, contours1, hierarchy1, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		 
		 findContours(imgThresholded2, contours2, hierarchy2, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		 vector<Moments> mu1(contours1.size() );
		 vector<Moments> mu2(contours2.size() );
		 for( int i = 0; i < contours1.size(); i++ ) {
		 	mu1[i] = moments( contours1[i], false );
		 }
		for( int i = 0; i < contours2.size(); i++ ) {
			mu2[i] = moments( contours2[i], false );
		 }
		 vector<Point2f> mc2( contours2.size() );
		 vector<Point2f> mc1( contours1.size() );

		 //cerr<<contours1.size()<<" "<<contours1.size()<<endl;
		for( int i = 0; i < contours2.size(); i++ ) {
		 	mc2[i] = Point2f( mu2[i].m10/mu2[i].m00 , mu2[i].m01/mu2[i].m00 );
		 	//cerr<<"mc2["<<i<<"]="<<mc2[i]<<endl; 
		 }
		 if(contours1.size()==2){
		 for( int i = 0; i < 2; i++ ) {
		 	mc1[i] = Point2f( mu1[i].m10/mu1[i].m00 , mu1[i].m01/mu1[i].m00 );
		 	//cerr<<"mc1["<<i<<"]="<<mc1[i]<<endl;
		 }	
		if(mu1[0].m00>mu1[1].m00&&count1==0)
			{ cerr<<"com of solid1 is "<<mc1[0]<<". com of ring1 is "<<mc1[1]<<endl;count1++;}
		if(mu1[0].m00<mu1[1].m00&&count1==0)
		 	{ cerr<<"com of solid1 is "<<mc1[1]<<". com of ring1 is "<<mc1[0]<<endl;count1++;}
		}

		 if(contours2.size()==2){
		 for( int i = 0; i < 2; i++ ) {
		 	mc2[i] = Point2f( mu2[i].m10/mu2[i].m00 , mu2[i].m01/mu2[i].m00 );
		 }	
		 if(mu2[0].m00>mu2[1].m00&&count2==0)
		 	{ cerr<<"com of solid2 is "<<mc2[0]<<". com of ring2 is "<<mc2[1]<<endl;count2++;}
		 if(mu1[0].m00<mu1[1].m00&&count1==0)
		 	{ cerr<<"com of solid2 is "<<mc2[1]<<". com of ring2 is "<<mc2[0]<<endl;count2++;}
		}

		  Mat imgLines1 = Mat::zeros( original.size(), CV_8UC3 );
		  line(imgLines1, mc1[0], mc1[1], Scalar(0,255,255), 2);
		  original+=imgLines1;

		  Mat imgLines2 = Mat::zeros( original.size(), CV_8UC3 );
		 line(imgLines2, mc2[0], mc2[1], Scalar(0,0,255), 2);
		 original+=imgLines1;

		 imshow("orig.",original);
		if(waitKey(10)==27)	return 0;;
 }return 0;

}
