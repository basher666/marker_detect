#Marker detection

###Working
The program used rgb value thresholding to detect the rectangular marker, then it erodes the picture twice to get rid of the noisy blobs, it then does a canny edge detection and uses probabilistic Hough line transform to detect the line segments . Finally the line with the maximum length is found and that line is extended to completely to give the sense of orientation  of the rectangular marker.

![alt tag](marker1.png)

![alt tag](marker2.png)

###Instructions
To run the program, the test video must be copied into the repo folder. The CMakeLists.txt is configured to create an executable named "i.out" , the path to the video must be specified while running i.out.