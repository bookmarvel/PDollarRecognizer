// This code was created based off the pseudocode and JS code from 
// https://depts.washington.edu/acelab/proj/dollar/pdollar.html
// No AI was used in the creation of this file

#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

// List of Points
// vector<Point> points_list;

class PointCloud;

//
// Point class
//
class Point {
	public:
		double X;
		double Y;
		int strokeID;  // stroke ID to which this point belongs (1,2,3,etc.)
	
	// constructor:
	Point(double x, double y, int id);
	
};


//
// Result class
//
class Result {
	public:
		string name;
		double score;
		time_t time;

	Result(string n, double sc, time_t ms);
};

//
// PDollarRecognizer constants
//
const int NumPointClouds = 0; 	// number of predefined gestures 
								// 16 given but they're all commented out
const int NumPoints = 32;
const Point Origin = Point(0,0,0);


//
// PDollarRecognizer class
//
class PDollarRecognizer {

	// PDollarRecognizer things

	public:
	// stores the point cloud templates
	vector<PointCloud> pointClouds;

	int size();

	// constructor
	PDollarRecognizer();


	
	//
	// The $P Point-Cloud Recognizer API begins here ------------
	//

	//
	// 3 methods: Recognize(), AddGesture(), DeleteUserGestures()
	//

	Result Recognize(vector<Point> points);


	int AddGesture(string Name, vector<Point>points);


	void DeleteUserGestures();


	//
	// Private helper functions
	//

	double GreedyCloudMatch(vector<Point> points, vector<Point> templ);

	double CloudDistance(vector<Point> pts1, vector<Point>pts2, int start);

	vector<Point> Resample(vector<Point> points, int n);

	vector<Point> Scale(vector<Point> points);

	vector<Point> TranslateTo(vector<Point> points, Point pt);

	Point Centroid(vector<Point> points);

	double Pathsize(vector<Point> points);

	double Distance(Point p1, Point p2);

}; // end PDollarRecognizer class


//
// PointCloud class: a point-cloud template
//
class PointCloud {
	public:
	string name;
	vector<Point> points;
	PDollarRecognizer recognizer;

	PointCloud(string n, vector<Point> p);
	PointCloud();
};