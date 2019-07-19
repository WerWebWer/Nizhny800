#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <cstring>
#include < string.h >
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "source.h"
#include <chrono>
#include "picture_and_probability.h"
using namespace cv;
using namespace std;

#define RATIO    0.75
#define count_r    31
#define ratio_thresh 0.75f
#define minHessian 1000000
#define scaleFactor 1.2f
#define nlevels 8
#define edgeThreshold count_r
#define firstLevel 0
#define WTA_K 2
#define scoreType ORB::HARRIS_SCORE
#define patchSize count_r
#define fastThreshold 20
#define count_size 300

string name_database(string s) {
	char* char_array = new char[s.length() + 1];
	//strcpy(char_array, s.c_str());
	//string str = string(arr);
	bool b = false;

	int j = 0;
	for (int i = 0; i < s.length() + 1; i++) {
		if (s[i] == '\\') {
			i++;
			b = true;
		}
		if (b) {
			for (j; ; j++, i++) {
				if (s[i] == '\\') break;
				char_array[j] = s[i];
			}
			i++;
			break;
		}
	}
	char* res = new char[j];
	for (int i = 0; i < j; i++) res[i] = char_array[i];
	delete[] char_array;
	string k = string(res);
	delete[] res;
	k.erase(j, 100);
	return k;
}

int id_database(string s) {
	char* char_array = new char[s.length() + 1];
	strcpy(char_array, s.c_str());
	return char_array[s.length() - 5];
}

void draw_histogram(Mat img, int i, string s) {
	Mat src = img;

	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true, accumulate = false;
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			(s == "RGB") ? Scalar(255, 0, 0) : Scalar(255, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			(s == "RGB") ? Scalar(0, 255, 0) : Scalar(0, 255, 255), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			(s == "RGB") ? Scalar(0, 0, 255) : Scalar(255, 0, 255), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1) + g_hist.at<float>(i - 1) + r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i) + g_hist.at<float>(i) + r_hist.at<float>(i))),
			Scalar(255, 255, 255), 2, 8, 0);
	}
	//imshow("Source image", src);
	imshow("histogram " + s + " " + to_string(i + 1), histImage);
	//waitKey();
}

void matchFeatures(const cv::Mat& query, const cv::Mat& target,
	std::vector<cv::DMatch>& goodMatches) {
	std::vector<std::vector<cv::DMatch>> matches;
	cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create();
	// Find 2 best matches for each descriptor to make later the second neighbor test.
	//std::cout << query;
	//std::cout << target;

	matcher->knnMatch(query, target, matches, 4);
	// Second neighbor ratio test.
	for (unsigned int i = 0; i < matches.size(); ++i) {
		if (matches[i][0].distance < matches[i][1].distance * RATIO)
			goodMatches.push_back(matches[i][0]);
	}
}
//img 

int goodMatchers(Mat img, Mat descriptors_object, vector<KeyPoint> keypoints_object, Picture_and_Probability& database) {
	//vector<KeyPoint > keypoints;
	//Mat descriptors;
	vector< vector<DMatch> > knn_matches;
	Ptr<BFMatcher> matcher = BFMatcher::create();
	vector<DMatch > good_matches;
	vector<Point2f> obj;
	vector<Point2f > scene;
	Mat img_matches;
	//draw HISTOGRAM RGB
	//draw_histogram(img[k], k, "RGB");
	//cvtColor(img[k], img[k], COLOR_RGB2HSV);
	//draw HISTOGRAM HSVs
	//draw_histogram(img[k], k, "HSV");

	matcher->knnMatch(descriptors_object, database.descriptor, knn_matches, 2);
	for (size_t i = 0; i < knn_matches.size(); i++) {
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	/*drawMatches(img, keypoints_object, database, keypoints, good_matches, img_matches, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	*/
	//for (size_t i = 0; i < good_matches.size(); i++) {
	//	//-- Get the keypoints from the good matches
	//	obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
	//	scene.push_back(keypoints[good_matches[i].trainIdx].pt);
	//	//cout << good_matches_naw[i].distance << " ";
	//}
	return good_matches.size();
}

void generate_DescriptorAndKeypoints(Picture_and_Probability& img, Ptr<ORB>& detector) {
	detector->detectAndCompute(img.image, noArray(), img.keypoints, img.descriptor);
}

void generate_Databse(Ptr<ORB>& detector, vector <Picture_and_Probability>& res) {
	auto start = std::chrono::system_clock::now();
	int h = -1;
	string str = "";
	string path("C:/database/*.jpg"); //select only jpg
	vector<string> fn;
	//vector <Picture_and_Probability> *res;
	/*cv::glob(path, fn, true);*/
	//fn.push_back("c:/database/1.png");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Alexander Nevsky Cathedral\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Bank Rukavishnikova\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Intel\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Kremlin Dmitrievskaya Tower\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Manor Rukavishnikova\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\National Bank\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Nizhny Novgorod Fair\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Palace of Labor\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Regional Court\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\Stadium Nizhny Novgorod\\" + to_string(i) + ".jpg");
	for (int i = 1; i < 9; i++)
		fn.push_back("C:/database\\UNN Faculty of Physics\\" + to_string(i) + ".jpg");
	std::cout << fn[1];

	for (int k = 0; k < fn.size(); ++k) {
		std::string s = fn[k];
		Mat im = imread(s.c_str(), 1);
		if (im.empty()) {
			cout << "Picture is empty" << endl;
			//return;
		}
		if (im.type() != CV_8UC3) {
			cout << "Picture is not CV_8UC3" << endl;
			//return;
		}
		Picture_and_Probability ob;
		if (str != name_database(s)) {
			str = name_database(s);
			h++;
		}
		ob.folder = str;
		ob.id_folder = h;
		ob.image = im;
		ob.name = id_database(s);
		res.push_back(ob);
	}
	h++;
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> t = end - start;
	std::cout << "databse start " << to_string(t.count()) << std::endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < res.size(); i++) {
		resize(res[i].image, res[i].image, Size(count_size * ((float)res[i].image.cols / (float)res[i].image.rows), count_size));
		generate_DescriptorAndKeypoints(res[i], detector);
		cout << i << "\n";
	}
	end = std::chrono::system_clock::now();
	t = end - start;
	std::cout << "databse is ready " << to_string(t.count()) << std::endl;
	//vector <Picture_and_Probability> *ress = &res;
	//return res;
}

//Ptr<ORB> generate_detector() {
	//Ptr<ORB> detector = ORB::create(minHessian, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize, fastThreshold);
	//Ptr<ORB> detector = ORB::create();
	//return detector;
//}

string showResult(string path, vector <Picture_and_Probability>& database) {
	Mat img_object = imread(path); //"../../nizhniy-800/database\\Alexander Nevsky Cathedral\\1.jpg"\
	string path("../../nizhniy-800/database/*.jpg"); //select only jpg
	///
		//vector <Picture_and_Probability> database = generate_Databse();
	///

	if (img_object.empty()) {
		cout << "Pictere is empty" << endl;
		//return;
	}
	if (img_object.type() != CV_8UC3) {
		cout << "Pictere is not CV_8UC3" << endl;
		//return;
	}

	resize(img_object, img_object, Size(count_size * ((float)img_object.cols / (float)img_object.rows), count_size));

	vector<KeyPoint> keypoints_object;
	Mat descriptors_object;

	int n = database.size();
	int h = database[n - 1].id_folder + 1;

	Ptr<ORB> detector = ORB::create(500);
	detector->detectAndCompute(img_object, noArray(), keypoints_object, descriptors_object);

	//draw_histogram(img_object, -1, "RGB");
	//cvtColor(img_object, img_object, COLOR_RGB2HSV);
	//draw_histogram(img_object, -1, "HSV");
	//vector<Mat> H;
	//for (int i = 0; i < n; i++) H.push_back(Mat());

	for (int k = 0; k < n; k++) {
		database[k].probability = goodMatchers(img_object, descriptors_object, keypoints_object, database[k]);

		//-- Draw matches
		//-- Localize the object
		//cout << "\n\n";
		//H[k] = findHomography(obj, scene[k], RANSAC);
		//-- Get the corners from the image_1 ( the object to be "detected" )
		//std::vector<Point2f> obj_corners(4);
		//obj_corners[0] = Point2f(0, 0);
		//obj_corners[1] = Point2f((float)img_object.cols, 0);
		//obj_corners[2] = Point2f((float)img_object.cols, (float)img_object.rows);
		//obj_corners[3] = Point2f(0, (float)img_object.rows);
		//std::vector<Point2f> scene_corners(4);
		//perspectiveTransform(obj_corners, scene_corners, H[k]);
		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		//line(img_matches[k], scene_corners[0] + Point2f((float)img_object.cols, 0),
		//	scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
		//line(img_matches[k], scene_corners[1] + Point2f((float)img_object.cols, 0),
		//	scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
		//line(img_matches[k], scene_corners[2] + Point2f((float)img_object.cols, 0),
		//	scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
		//line(img_matches[k], scene_corners[3] + Point2f((float)img_object.cols, 0),
		//	scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
		//-- Show detected matches
		//std::cout << "Good matches for <" << res[k].folder << " " << res[k].name << "> " << res[k].probability << std::endl;
		//imshow("Good Matches & Object detection " + to_string(k+1), img_matches[k]);
		//Sleep(100);//2 секунд
		//waitKey();
	};
	//end = std::chrono::system_clock::now();
	//t = end - start;
	//std::cout << to_string(t.count()) << std::endl;

	std::sort(database.begin(), database.end(), [](const Picture_and_Probability& a, const Picture_and_Probability& b) -> bool {
		return a.probability > b.probability;
	}
	);
	for (int i = 0; i < n / h; ++i)
		std::cout << "No " << i << ": " << database[i].id_folder << " " << database[i].folder << " " << database[i].name << " " << database[i].probability << '\n';

	int* mas = new int[h];
	for (int i = 0; i < h; i++) mas[i] = 0;
	for (int i = 0; i < n / h; i++) mas[database[i].id_folder]++;

	cout << "\n\n";
	for (int i = 0; i < h; i++) cout << mas[i] << " ";
	int best_res = -1;
	int id_best_res = -1;
	int count_similary = 0;
	for (int i = 0; i < h; i++) {
		if (best_res < mas[database[i].id_folder]) {
			best_res = mas[database[i].id_folder];
			id_best_res = database[i].id_folder;
			count_similary++;
		}
	}

	if (count_similary > 1) {
		int* mas_better = new int[h];
		int k = 0;
		for (int i = 0; i < h; i++) {
			if (best_res == mas[i]) {
				mas_better[k] = mas[i];
				k++;
			}
		}
		int j = 0;
		for (int i = 0;;) {
			if (database[j].id_folder == mas_better[i]) {
				id_best_res = database[i].id_folder;
				break;
			}
			else {
				j++;
			}
		}
	}
	cout << "\n\nBEST IS " << id_best_res;
	cout << "\n";
	int i;
	for (i = 0;; i++)
		if (database[i].id_folder == id_best_res) {
			cout << database[i].folder;
			break;
		}

	//cv::namedWindow(database[i].folder, cv::WINDOW_NORMAL);
	//cv::imshow(database[i].folder, database[i].image);
	//waitKey();
	return database[i].folder;
}
