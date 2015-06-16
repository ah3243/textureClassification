/*Copyright 2015*/
#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>  // used for dir creation
#include <sys/stat.h>   // used for dir creation

#include <iostream>     // std::cout
#include <string>       // std::string
#include <sstream>      // std::stringstream , std::stringbuffer
#include <ctime>        // for time and date


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


// Global variables
int kernelSize = 21;
int imgCount = 0;
cv::Mat src, dst, dst1, dst2, input;

// Apply Edge/Bar filter
void process(int x, int y);

// Save images to folder
int saveImg(Mat img, string imgType);


int main(int argc, char** argv) {
  // Create variables
  int ddepth = CV_16S;
  string window_name1 = "Gaussian";
  string window_name2 = "Laplacian of Gaussian";
  string window_name3 = "Anisometric Filters";

  // Load image in grayscale
  input = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  // Check that data is read in
  if (input.empty()) {
    cout << "the input image was unable to be read" << endl;
    return -1;
  }

  // convert color img to grayscale
  cvtColor(input, input, CV_BGR2GRAY);

  // Create a named window
  namedWindow(window_name1, WINDOW_AUTOSIZE);

/* ------------------------ Show original image ------------------------ */
  const char *window_name = "Source image";
  imshow(window_name, input);

  waitKey();

/* ---------------- Equalise Histogram and show image ------------------ */
  // Equalise the input image, save in src
  equalizeHist(input, src);

  // Show equalized image
  imshow(window_name, src);
  waitKey();

/* ------------------------ Gaussian Filter ----------------------------- */
  // Blur original image and display
  GaussianBlur(src, dst, Size(11, 11), 0, 0, BORDER_DEFAULT);

  // Save image
  saveImg(dst, "Gaussian_filter");

  // Show dst Mat in created window
  imshow(window_name1, dst);


/* ------------------------ Laplician of Gaussian -------------------------- */
  //LoG output hold Mat
  Mat SclConv;

  // Apply Laplacian to Blured image and display
  Laplacian(src, dst1, ddepth, 11, 1, 0, BORDER_DEFAULT);

  // Convert output from Laplacian operator to CV_8U image
  convertScaleAbs(dst1, SclConv);

  // Show dst Mat in created window
  imshow(window_name2, dst1);

  // Save image
  imgCount = 0;
  saveImg(dst1, "LoG_filter");

  // Wait for anykey before exit
  waitKey();

/* ------------------------ Bar and Edge filters -------------------------- */
  double filterAngle[6] = {1, 3, 2, 6, 4, 12};

  // time_t t = time(0);   // get time now
  // // Print out current time and date
  //     struct tm * now = localtime(& t);
  //     cout << (now->tm_year + 1900) << '-'
  //          << (now->tm_mon + 1) << '-'
  //          <<  now->tm_mday
  //          << endl;


  imgCount = 0;
  for (int i = 1; i < 7; i+=2) {
    process(filterAngle[i], filterAngle[i+1]);

    // Save image
    saveImg(dst2, "bar_Fitler");

    // Display Kernel
    imshow("Gabor Filter", dst2);
    waitKey(0);
  }

  return 0;
}

void process(int x, int y) {
  GaussianBlur(src, dst2, Size(), x, y, BORDER_DEFAULT);
}

int saveImg(Mat img, string imgType) {
  string imgpath = "../savImgs/test";

  string fullpath = "mkdir " + imgpath;
  system(fullpath.c_str());   // create dir for filtered images

  std::stringstream ss;

  std::string imgname = "/" + imgType;
  std::string type = ".png";

  ss << imgpath << imgname << imgCount << type;
  std::string fileName = ss.str();
  ss.str("");

  // Print out filename
  cout << "This is the filename " << fileName << endl;
  imwrite(fileName, img);
  imgCount++;
}
