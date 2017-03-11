#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv/cv.h"
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

double Slope(int x0, int y0, int x1, int y1)
{
  return (double)(y1-y0)/(x1-x0);
}

void fullLine(Mat *img, Point a, Point b)
{
  double slope = Slope(a.x, a.y, b.x, b.y);
  cout<<slope<<endl;
  Scalar color(255,0,0);
  
  Point p(0,0), q(img->cols,img->rows);
  
  if(a.x!=b.x)
    {
      p.y = -(a.x - p.x) * slope + a.y;
      q.y = -(b.x - q.x) * slope + b.y;
    }
  else
    {
      p.x=a.x;
      q.x=b.x;
    }
  line(*img,p,q,color,2,8);
}
/*
Mat conv2bin(Mat frame,int b,int g,int r)
{
  Mat binary(frame.rows,frame.cols,CV_8UC1);
  for(int i=0;i<frame.rows;i++)
    {
      for(int j=0;j<frame.cols;j++)
	{
	  Vec3b intensity=frame.at<Vec3b>(i,j);
	  if(intensity.val[0]>=b && intensity.val[1]>=g && intensity.val[2]>=r)
	    {
	      binary.at<uchar>(i,j)=255;
	    }
	  else
	    binary.at<uchar>(i,j)=0;
	}
    }
  return binary;
  

}
*/

float calc_dist(Point a,Point b) //calculates the distance between two points
{
  float ret=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
  return ret;
}

int main(int argc,char **argv)
{

  char *file_name=argv[1];
  VideoCapture cap(file_name);

  int hue_max=20,hue_min=0,sat_min=70,sat_max=255,val_min=0,val_max=255;

  namedWindow("hue,saturation,value threshold");

  createTrackbar("Hue_lower","hue,saturation,value threshold",&hue_min,255,NULL);
  createTrackbar("Hue_upper","hue,saturation,value threshold",&hue_max,255,NULL);
  createTrackbar("Sat_lower","hue,saturation,value threshold",&sat_min,255,NULL);
  createTrackbar("Sat_upper","hue,saturation,value threshold",&sat_max,255,NULL);
  createTrackbar("Val_lower","hue,saturation,value threshold",&val_min,255,NULL);
  createTrackbar("Val_upper","hue,saturation,value threshold",&val_max,255,NULL);
  while(1)
    {
      
      Mat frame,dst,ldst,ero,hsv,binary;
      
      vector<vector <Point > > contours;
      vector<Vec4i > hierarchy;
      
      cap>>frame;
      
      //Mat binary=conv2bin(frame,b,g,r);

      cvtColor(frame,hsv,COLOR_BGR2HSV);

      inRange(hsv,Scalar(hue_min,sat_min,val_min),Scalar(hue_max,sat_max,val_max),binary);
      
      Mat element=getStructuringElement(MORPH_RECT,Size(3,3));
      //erode(binary,ero,element);
      ero=binary.clone();
      erode(ero,binary,element);

      /*
      findContours(ero,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
      //cout<<contours.size()<<endl;
      
      Mat drawing = Mat::zeros(frame.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
	{
	  Scalar color = Scalar(255,0,0 );
	  drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}
      
      */
	
      Canny(binary,ldst,60,180,3);

      
      vector<Vec4i> lines;
      
      HoughLinesP(ldst,lines,2,CV_PI/360,80,10,30);
      
      for(size_t i=0;i<lines.size();i++)
	{
	  line(frame,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,255,0),3,8);
	}
      Point max_1,max_2;
      float max_dist=0;
      
      for(size_t i=0;i<lines.size();i++)
	{
	  
	  float t =calc_dist(Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]));
	  if(t>max_dist)
	    {
	      max_dist=t;
	      max_1.x=lines[i][0];
	      max_1.y=lines[i][1];
	      max_2.x=lines[i][2];
	      max_2.y=lines[i][3];

	    }
	}

      fullLine(&frame, max_1,max_2);
      line(frame,max_1,max_2,Scalar(255,0,0),3,8);

      imshow("hue,saturation,value threshold",frame);
      imshow("erosion",binary);
      //imshow("canny",ldst);

      char ch=waitKey(1000);
      
      if(ch=='q')
	break;
      else if(ch=='p')
	for(;;);
    }
  return 0;
}
