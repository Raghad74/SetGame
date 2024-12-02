#ifndef HELPER_H
#define HELPER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Set.h"

/*
getContours- given an image and a filterVal , returns countours of said image 
--the returned contours are only those whose area is larger than filterVal
*/
std::vector<std::vector<cv::Point>>* getContours(cv::Mat& image,int filterVal=0);
/*
getCards- 
*/
std::vector<cv::Rect>* getCards(cv::Mat& img);
Feature getShape(cv::Mat& card);
Feature getColor(cv::Mat& card,double* colored_pixeled_num);
Feature getFilling(cv::Mat& item,double colored_pixels_num);

#endif



