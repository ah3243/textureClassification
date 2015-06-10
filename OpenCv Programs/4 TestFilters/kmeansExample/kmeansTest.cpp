/*Resource "http://stackoverflow.com/questions/10240912/input-matrix-to-opencv-kmeans-clustering"*/
#include <stdio.h>
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
  // Read grayscale image in
  Mat in, src;
  in = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
//  namedWindow("clustered image", WINDOW_AUTOSIZE);

  // Confirm image found
  if (!in.data) {
    cout << "No input data found exiting" << endl;
    return -1;
  }

  // Equalise input's histogram
  equalizeHist(in, src);

  // Convert input image to a float
  src.convertTo(src, CV_32F);

  // Create sample Mat, same size as input image
  Mat samples(src.rows * src.cols, 1, CV_32F);

  // Copy across input image
  for (int y = 0; y < src.rows; y++)
    for (int x = 0; x < src.cols; x++)
      samples.at<float>(y, x) = src.at<float>(y, x);

  // KMeans parameters
  int clusterCount = 10;
  Mat labels;
  int attempts = 5;
  Mat centers;
  // Apply KMeans
  kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);


cout << "This is the cluster centers: " << endl << centers << endl;

  //  Mat new_image(src.size(), src.type() );
  // for (int y = 0; y < src.rows; y++) {
  //   for (int x = 0; x < src.cols; x++) {
  //     int cluster_idx = labels.at<int>(y + x*src.rows, 0);
  //     new_image.at<float>(y, x) = centers.at<float>(cluster_idx);
  //     // new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
  //     // new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
  //   }
  // }
  //
  // double minVal, maxVal;
  // minMaxLoc(src, &minVal, &maxVal);
  // Mat draw;
  // new_image.convertTo(draw, CV_8U, 255.0/(maxVal-minVal), -minVal * 255.0/(maxVal-minVal));
  //
  // imshow("clustered image", draw);
  waitKey(0);
}
