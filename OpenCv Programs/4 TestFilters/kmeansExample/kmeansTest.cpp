
#include <dirent.h>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>  // Used for imgpath string stream

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


// Check that file in dir is an accepted img type
bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

cv::Mat kmeansFunc(cv::Mat samples) {
  // KMeans parameters
  int clusterCount = 10;
  cv::Mat labels;
  int attempts = 5;
  cv::Mat centers;

  // Apply KMeans
  kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
  return centers;
}

cv::Mat createSamples(cv::Mat src) {
  // Create sample` Mat`, same size as input image
  cv::Mat samples(src.rows * src.cols, 1, CV_32F);

  // Copy across input image
  for (int y = 0; y < src.rows; y++) {
    for (int x = 0; x < src.cols; x++) {
      samples.at<float>(y, x) = src.at<float>(y, x);
    }
  }

  // Put combined Mat through kmeans
  return kmeansFunc(samples);
}

int main(int argc, char** argv) {
  namedWindow("testWindow", 1);

/*-------------- Load All images from Folder -----------------------*/
  cv::Mat img, aggImg;

  std::string extTypes[] = {".jpg", ".png", ".bmp"};
  std::string dirNme = "../savImgs/test/";
  DIR *dir;
  dir = opendir(dirNme.c_str());
  string imgName;
  struct dirent *ent;

  int cnt = 0;
  if (dir != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      imgName = ent->d_name;
      if (hasEnding(imgName, ".jpg")) {
        // Sort out string Stream
        std::stringstream ss;
        ss << dirNme << imgName;
        std::string imgpath = ss.str();

        // Read in image
       cv::Mat in = imread(imgpath, CV_LOAD_IMAGE_GRAYSCALE);
        // Equalise image
        equalizeHist(in, img);
        // Print out importfile Name
        cout << "importing File: " << imgpath << endl;
        // Populate aggImg if first import
        if (cnt ==0) {
          aggImg = img;
        } else {
          // Blend images
          addWeighted(aggImg, 0.5, img, 0.5, 0.0, aggImg);
        }

        imshow("testWindow", aggImg);

      } else {
      // Printout err if incorrect extension
      cout << "wrong extension detected: " << imgName << endl;
      }
      cnt++;
    }
    closedir(dir);
  } else {
    cout << "dir not found" << endl;
  }

/*--------------------- Cluster aggregated images --------------------------*/
  waitKey();

  // Convert input image to a float
  aggImg.convertTo(aggImg, CV_32F);
  cv::Mat centers3 = createSamples(aggImg);
  cout << "This is the cluster centers of two images: " << endl << centers3 << endl;

  return 0;
}
