/*Copyright 2015*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;


/*Gabor filter globals*/
int kernelSize = 21;
double sigma = 10, theta = 1, lambd = 20, gm = 0, psi = 0;
cv::Mat dst, dst1, dst_gab, src, src_gab, in, output, kernel, kernel_gab;

//Gabor filter function
void process(double angle, void *) {
  theta = angle;
  kernel_gab = cv::getGaborKernel(cv::Size(kernelSize, kernelSize), sigma, theta, lambd, gm, psi);
  cv::filter2D(src_gab, dst_gab, CV_32F, kernel_gab);
}


int main(int argc, char** argv) {

  // Create variables
  Point anchor;
  double delta;
  int ddepth = CV_16S;
  const char *window_name1 = "Gaussian";
  const char *window_name2 = "Laplacian of Gaussian";
  const char *window_name3 = "GaborFilters";

  // Load image in grayscale
  src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  // Check that data is read in correctly
  if (!src.data) {
    return -1;
  }

  // Create a named window
  namedWindow(window_name1, WINDOW_AUTOSIZE);


  /* Shows original image*/
  const char *window_name = "Source image";
  imshow(window_name, src);

  // // Initialise filter arguments
  // anchor = Point(-1, -1);
  // delta = 0;
  // ddepth = -1;
  //
  // kernel = (Mat_<char>(3, 3) << 1, 1, 1,
  //                              1,  1, 1,
  //                               1, 1,  1);
  //
  // // Run the Filter
  // filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

  // Blur original image and display
  GaussianBlur(src, dst, Size(11, 11), 1, 0, BORDER_DEFAULT);

  // Show dst Mat in created window
   imshow(window_name1, dst);

  Mat SclConv;
  // Apply Laplacian to Blured image and display

  Laplacian(dst, dst1, ddepth, 11, 1, 0, BORDER_DEFAULT);

  // Convert output from Laplacian operator to CV_8U image
  convertScaleAbs(dst1, SclConv);

  // Show dst Mat in created window
   imshow(window_name2, dst1);

  // Wait for anykey before exit
  waitKey(0);


/*Gabor Filters*/
src.convertTo(src_gab, CV_32F);

string imgDir ="";

for (int i = 0; i < 40; i++) {
  double th = i;
  // Print out theta conv to int
  cout << "This is the theta number " << th << endl;

  process(th/10, 0);
  dst_gab.convertTo(output, CV_8U, 1.0/255.0);

  // Display Kernel
  imshow(window_name3, kernel_gab);
  imshow("Gabor Filter", output);

  imgDir.assign("../savImgs/GaborImg23.png");

  imwrite(imgDir, output);

  waitKey();
}

  return 0;
}
