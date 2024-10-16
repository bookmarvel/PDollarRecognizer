// This code was created based off the pseudocode and JS code from 
// https://depts.washington.edu/acelab/proj/dollar/pdollar.html
// No AI was used in the creation of this file

#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <float.h>
#include "pdollar.h"

using namespace std;

// List of Points
// vector<Point> points_list;


// Point constructor
Point::Point(double x, double y, int id) {
	X = x;
	Y = y;
	strokeID = id;
}


// PointCloud constructor
PointCloud::PointCloud(string n, vector<Point> p) {
	name = n;
	recognizer = PDollarRecognizer();
	vector<Point> temp = this->recognizer.Resample(p, NumPoints);
	temp = this->recognizer.Scale(temp);
	temp = this->recognizer.TranslateTo(temp, Point(0,0,0));
	points = temp;
}

PointCloud::PointCloud() {
	name = "";
	points = {};
	recognizer = PDollarRecognizer();
}


// Result constructor
Result::Result(string n, double sc, time_t ms) {
	name = n;
	score = sc;
	time = ms;
};


// PDollarRecognizer 

int PDollarRecognizer::size() {
	return pointClouds.size();
}

// constructor
PDollarRecognizer::PDollarRecognizer() {
	pointClouds = vector<PointCloud>(NumPointClouds);

	//
	// add all predefined gestures
	// one point-cloud for each gesture
	//

	// for this assignment we don't want them tho

/*
	// gesture "T"
	pointClouds[0] = PointCloud("T", vector<Point>({
		Point(30,7,1), Point(103,7,1),
		Point(66,7,2), Point(66,87,2)})
	);

	// gesture "N"
	pointClouds[0] = PointCloud("N", vector<Point>({
		Point(177,92,1), Point(177,2,1),
		Point(182,1,2), Point(246,95,2),
		Point(247,87,3), Point(247,1,3)
		})
	);

	// gesture "D"
	pointClouds[0] = PointCloud("D", vector<Point>({
		Point(345,9,1), Point(345,87,1),
		Point(351,8,2), Point(363,8,2), Point(372,9,2), Point(380,11,2), Point(386,14,2), Point(391,17,2), Point(394,22,2), Point(397,28,2), Point(399,34,2), Point(400,42,2), Point(400,50,2), Point(400,56,2), Point(399,61,2), Point(397,66,2), Point(394,70,2), Point(391,74,2), Point(386,78,2), Point(382,81,2), Point(377,83,2), Point(372,85,2), Point(367,86,2), Point(360,87,2), Point(355,87,2), Point(349,86,2)
		})
	);

	// gesture "P"
	pointClouds[0] = PointCloud("P", vector<Point>({
		Point(507,8,1), Point(507,87,1),
		Point(513,7,2), Point(528,7,2), Point(537,8,2), Point(544,10,2), Point(550,12,2), Point(555,15,2), Point(558,18,2), Point(560,22,2), Point(561,27,2), Point(562,33,2), Point(561,37,2), Point(559,42,2), Point(556,45,2), Point(550,48,2), Point(544,51,2), Point(538,53,2), Point(532,54,2), Point(525,55,2), Point(519,55,2), Point(513,55,2), Point(510,55,2)
		})
	);
	
	this.PointClouds[4] = new PointCloud("X", new Array(
		new Point(30,146,1),new Point(106,222,1),
		new Point(30,225,2),new Point(106,146,2)
	));
	this.PointClouds[5] = new PointCloud("H", new Array(
		new Point(188,137,1),new Point(188,225,1),
		new Point(188,180,2),new Point(241,180,2),
		new Point(241,137,3),new Point(241,225,3)
	));
	this.PointClouds[6] = new PointCloud("I", new Array(
		new Point(371,149,1),new Point(371,221,1),
		new Point(341,149,2),new Point(401,149,2),
		new Point(341,221,3),new Point(401,221,3)
	));
	this.PointClouds[7] = new PointCloud("exclamation", new Array(
		new Point(526,142,1),new Point(526,204,1),
		new Point(526,221,2)
	));
	this.PointClouds[8] = new PointCloud("line", new Array(
		new Point(12,347,1),new Point(119,347,1)
	));
	this.PointClouds[9] = new PointCloud("five-point star", new Array(
		new Point(177,396,1),new Point(223,299,1),new Point(262,396,1),new Point(168,332,1),new Point(278,332,1),new Point(184,397,1)
	));
	this.PointClouds[10] = new PointCloud("null", new Array(
		new Point(382,310,1),new Point(377,308,1),new Point(373,307,1),new Point(366,307,1),new Point(360,310,1),new Point(356,313,1),new Point(353,316,1),new Point(349,321,1),new Point(347,326,1),new Point(344,331,1),new Point(342,337,1),new Point(341,343,1),new Point(341,350,1),new Point(341,358,1),new Point(342,362,1),new Point(344,366,1),new Point(347,370,1),new Point(351,374,1),new Point(356,379,1),new Point(361,382,1),new Point(368,385,1),new Point(374,387,1),new Point(381,387,1),new Point(390,387,1),new Point(397,385,1),new Point(404,382,1),new Point(408,378,1),new Point(412,373,1),new Point(416,367,1),new Point(418,361,1),new Point(419,353,1),new Point(418,346,1),new Point(417,341,1),new Point(416,336,1),new Point(413,331,1),new Point(410,326,1),new Point(404,320,1),new Point(400,317,1),new Point(393,313,1),new Point(392,312,1),
		new Point(418,309,2),new Point(337,390,2)
	));
	this.PointClouds[11] = new PointCloud("arrowhead", new Array(
		new Point(506,349,1),new Point(574,349,1),
		new Point(525,306,2),new Point(584,349,2),new Point(525,388,2)
	));
	this.PointClouds[12] = new PointCloud("pitchfork", new Array(
		new Point(38,470,1),new Point(36,476,1),new Point(36,482,1),new Point(37,489,1),new Point(39,496,1),new Point(42,500,1),new Point(46,503,1),new Point(50,507,1),new Point(56,509,1),new Point(63,509,1),new Point(70,508,1),new Point(75,506,1),new Point(79,503,1),new Point(82,499,1),new Point(85,493,1),new Point(87,487,1),new Point(88,480,1),new Point(88,474,1),new Point(87,468,1),
		new Point(62,464,2),new Point(62,571,2)
	));
	this.PointClouds[13] = new PointCloud("six-point star", new Array(
		new Point(177,554,1),new Point(223,476,1),new Point(268,554,1),new Point(183,554,1),
		new Point(177,490,2),new Point(223,568,2),new Point(268,490,2),new Point(183,490,2)
	));
	this.PointClouds[14] = new PointCloud("asterisk", new Array(
		new Point(325,499,1),new Point(417,557,1),
		new Point(417,499,2),new Point(325,557,2),
		new Point(371,486,3),new Point(371,571,3)
	));
	this.PointClouds[15] = new PointCloud("half-note", new Array(
		new Point(546,465,1),new Point(546,531,1),
		new Point(540,530,2),new Point(536,529,2),new Point(533,528,2),new Point(529,529,2),new Point(524,530,2),new Point(520,532,2),new Point(515,535,2),new Point(511,539,2),new Point(508,545,2),new Point(506,548,2),new Point(506,554,2),new Point(509,558,2),new Point(512,561,2),new Point(517,564,2),new Point(521,564,2),new Point(527,563,2),new Point(531,560,2),new Point(535,557,2),new Point(538,553,2),new Point(542,548,2),new Point(544,544,2),new Point(546,540,2),new Point(546,536,2)
	));
*/

} // end PDollarRecognizer constructor
	
//
// The $P Point-Cloud Recognizer API begins here ------------
//

//
// 3 methods: Recognize(), AddGesture(), DeleteUserGestures()
//

Result PDollarRecognizer::Recognize(vector<Point> points) {
	time_t timeBegin = time(NULL);
	PointCloud candidate = PointCloud("", points);

	int resultIndex = -1;
	double score = DBL_MAX;

	for (int i = 0; i < this->size(); i++) // for each point-cloud template
	{
		int d = GreedyCloudMatch(candidate.points, this->pointClouds[i].points);
		if (score > d) {
			score = d; // best (least) distance
			resultIndex = i; // point-cloud index
		}
	}

	time_t timeEnd = time(NULL);

	double normalizedScore = max((2.0 - score)/2.0, 0.0);

	if (resultIndex == -1) {
		return Result("No match.", 0.0, timeEnd - timeBegin);
	} else {
		return Result(this->pointClouds[resultIndex].name, normalizedScore, timeEnd - timeBegin);
	}

} // end Recognize()


int PDollarRecognizer::AddGesture(string Name, vector<Point>points) {
	this->pointClouds.push_back(PointCloud(Name, points));
	int num = 0;
	for (int i = 0; i < this->size(); i++) {
		if (this->pointClouds[i].name == Name)
			num++;
	}
	return num;
}


void PDollarRecognizer::DeleteUserGestures() {
	// int size = NumPointClouds; // clears any beyond the original set

	// while (this->size() > size) {
	// 	this->pointClouds.pop_back();
	// }

	this->pointClouds.clear();
}


//
// Private helper functions 
//


double PDollarRecognizer::GreedyCloudMatch(vector<Point> points, vector<Point> templ) {
	double e = 0.50;
	int step = floor(pow(points.size(), 1.0 - e));
	double minAlign = DBL_MAX;
	for (int i = 0; i < points.size(); i += step) {
		double d1 = CloudDistance(points, templ, i);
		double d2 = CloudDistance(templ, points, i);
		minAlign = min(minAlign, min(d1, d2)); // min3
	}
	return minAlign; // minimum alignment cost
} // end GreedyCloudMatch()


double PDollarRecognizer::CloudDistance(vector<Point> pts1, vector<Point>pts2, int start) {
	vector<bool> matched = vector<bool>(pts1.size()); // pts1.size == pts2.size
	for (int k = 0; k < pts1.size(); k++)
		matched[k] = false;
	double sum = 0.0;
	int i = start;
	do
	{
		int index = -1;
		double minDist = DBL_MAX;
		for (int j = 0; j < matched.size(); j++)
		{
			if (!matched[j]) {
				double d = Distance(pts1[i], pts2[j]);
				if (d < minDist) {
					minDist = d;
					index = j;
				}
			}
		}
		matched[index] = true;
		double weight = 1 - ((i - start + pts1.size()) % pts1.size()) / pts1.size();
		sum += weight * minDist;
		i = (i + 1) % pts1.size();
	} while (i != start);

	return sum;
} // end CloudDistance()


vector<Point> PDollarRecognizer::Resample(vector<Point> points, int n) {
	double lenPath = Pathsize(points); 
	double I = lenPath / (n - 1); // interval size
	double D = 0.0;
	vector<Point> newpoints = vector<Point>({points[0]});
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].strokeID == points[i-1].strokeID)
		{
			double d = Distance(points[i-1], points[i]);
			if ((D + d) >= I)
			{
				double qx = points[i-1].X + ((I - D) / d) * (points[i].X - points[i-1].X);
				double qy = points[i-1].Y + ((I - D) / d) * (points[i].Y - points[i-1].Y);
				Point q = Point(qx, qy, points[i].strokeID);
				newpoints.push_back(q); // append new point 'q'
				points.insert(points.begin()+i, q); // insert 'q' at position i in points s.t. 'q' will be the next i
				D = 0.0;
			}
			else D += d;
		}
	}
	if (newpoints.size() == n - 1) { // sometimes we fall a rounding-error short of adding the last point, so add it if so
		newpoints.push_back(Point(points[points.size() - 1].X, points[points.size() - 1].Y, points[points.size() - 1].strokeID));
	}

	return newpoints;
} // end Resample()


vector<Point> PDollarRecognizer::Scale(vector<Point> points)
{
	double minX = DBL_MAX;
	double maxX = 0;
	double minY = DBL_MAX;
	double maxY = 0;
	for (int i = 0; i < points.size(); i++) {
		minX = min(minX, points[i].X);
		minY = min(minY, points[i].Y);
		maxX = max(maxX, points[i].X);
		maxY = max(maxY, points[i].Y);
	}
	double scale = max(maxX - minX, maxY - minY);
	if (scale != 0) {
		vector<Point> newpoints = {};
		for (int i = 0; i < points.size(); i++) {
			double qx = (points[i].X - minX) / scale;
			double qy = (points[i].Y - minY) / scale;
			newpoints.push_back(Point(qx, qy, points[i].strokeID));
		}
		return newpoints;
	} else {
		return points;
	}
} // end Scale()


vector<Point> PDollarRecognizer::TranslateTo(vector<Point> points, Point pt) // translates points' centroid to pt
{
	Point c = Centroid(points);
	vector<Point> newpoints = vector<Point>();
	for (int i = 0; i < points.size(); i++) {
		double qx = points[i].X + pt.X - c.X;
		double qy = points[i].Y + pt.Y - c.Y;
		newpoints.push_back(Point(qx, qy, points[i].strokeID));
	}
	return newpoints;
} // end TranslateTo()


Point PDollarRecognizer::Centroid(vector<Point> points)
{
	double x = 0.0;
	double y = 0.0;
	for (int i = 0; i < points.size(); i++) {
		x += points[i].X;
		y += points[i].Y;
	}
	x /= points.size();
	y /= points.size();
	return Point(x, y, 0);
} // end Centroid()


double PDollarRecognizer::Pathsize(vector<Point> points) // size traversed by a point path
{
	double d = 0.0;
	for (int i = 1; i < points.size(); i++) {
		if (points[i].strokeID == points[i-1].strokeID)
			d += Distance(points[i-1], points[i]);
	}
	return d;
} // end Pathsize()


double PDollarRecognizer::Distance(Point p1, Point p2) // Euclidean distance between two points
{
	double dx = p2.X - p1.X;
	double dy = p2.Y - p1.Y;
	return sqrt(dx * dx + dy * dy);
}
