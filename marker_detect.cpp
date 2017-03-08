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

  Scalar color(255,0,0);
  
  Point p(0,0), q(img->cols,img->rows);

  p.y = -(a.x - p.x) * slope + a.y;
  q.y = -(b.x - q.x) * slope + b.y;

  line(*img,p,q,color,2,8);
}

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


float calc_dist(Point a,Point b) //calculates the distance between two points
{
  float ret=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
  return ret;
}

int main(int argc,char **argv)
{

  char *file_name=argv[1];
  VideoCapture cap(file_name);
  int b=20,g=20,r=200;
  namedWindow("bgr threshold");

  createTrackbar("blue","bgr threshold",&b,255,NULL);
  createTrackbar("green","bgr threshold",&g,255,NULL);
  createTrackbar("red","bgr threshold",&r,255,NULL);
  
  while(1)
    {
      
      Mat frame,dst,ldst,ero;
      
      vector<vector <Point > > contours;
      vector<Vec4i > hierarchy;
      
      cap>>frame;
      //blur(frame,frame,Size(3,3));
      Mat binary=conv2bin(frame,b,g,r);
      
      Mat element=getStructuringElement(MORPH_RECT,Size(3,3));
      erode(binary,ero,element);
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
      
      HoughLinesP(ldst,lines,1,CV_PI/180,80,30,10);
      
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
      //cout<<max_1.y<<","<<max_2.y<<endl;

      imshow("bgr threshold",frame);
      imshow("erosion",binary);

      char ch=waitKey(1000);
      
      if(ch=='q')
	break;
      else if(ch=='p')
	for(;;);
    }
  return 0;
}
