#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <cstring>
#include < string.h >
#include "opencv2/opencv.hpp"
#include "picture_and_probability.h"

using namespace cv;
using namespace std;

std::string name_database(std::string s);

int id_database(std::string s);

void draw_histogram(cv::Mat img, int i, std::string s);

void matchFeatures(const cv::Mat& query, const cv::Mat& target, std::vector<cv::DMatch>& goodMatches);

int goodMatchers(Mat img, Mat descriptors_object, vector<KeyPoint> keypoints_object, Picture_and_Probability& database);

void generate_DescriptorAndKeypoints(Picture_and_Probability& img, Ptr<ORB>& detector);

string showResult(string path, vector <Picture_and_Probability>& database);

//Ptr<ORB> generate_detector();

void generate_Databse(Ptr<ORB>& detector, vector <Picture_and_Probability>& res);