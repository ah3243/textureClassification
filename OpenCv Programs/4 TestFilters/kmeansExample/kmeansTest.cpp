
#include <dirent.h>
#include <opencv2/core/core.hpp>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>  // Used for imgpath string stream

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


// Check that file in dir is an accepted img type
bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

Mat kmeansFunc(Mat samples){
  // KMeans parameters
  int clusterCount = 10;
  Mat labels;
  int attempts = 5;
  Mat centers;

  // Apply KMeans
  kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
  return centers;
}

void createSamples(Mat src) {
  // Create sample Mat, same size as input image
  Mat samples(src.rows * src.cols, 1, CV_32F);

  // Copy across input image
  for (int y = 0; y < src.rows; y++) {
    for (int x = 0; x < src.cols; x++) {
      samples.at<float>(y, x) = src.at<float>(y, x);
    }
  }

  // Put combined Mat through kmeans
  Mat centers2 = kmeansFunc(samples);
  cout << "This is the cluster centers of two images: " << endl << centers2 << endl;
  return;
}

int main(int argc, char** argv) {
  namedWindow("testWindow", 1);

/*-------------------------- Load All images from Folder ----------------------------*/
  cv::Mat img, aggImg;

  std::string extTypes[] = {".jpg", ".png", ".bmp"};
  std::string dirNme = "../../TEST_IMAGES/";
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
        Mat in = imread(imgpath, CV_LOAD_IMAGE_GRAYSCALE);
         equalizeHist(in, img);
         cout << "importing File: " << imgpath << endl;
        if (cnt ==0) {
          aggImg = img;
        } else {
          addWeighted(aggImg, 0.5, img, 0.5, 0.0, aggImg);
        }

        imshow("testWindow", aggImg);

      } else {
      cout << "wrong extension detected: " << imgName << endl;
      }
      cnt++;
    }
    closedir(dir);
  } else {
    cout << "dir not found" << endl;
  }

/*-------------------------- Cluster aggregated images ----------------------------*/
  waitKey();
  // Convert input image to a float
  aggImg.convertTo(aggImg, CV_32F);
  createSamples(aggImg);

  return 0;
}
