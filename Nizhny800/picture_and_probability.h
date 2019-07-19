#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <string.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

struct Picture_and_Probability {
	std::string folder;
	std::string name;
	int probability;
	int id_folder;
	Mat image;
	vector<KeyPoint> keypoints;
	Mat descriptor;
	string description;
};
