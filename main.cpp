// This code was written completely by me (Asher Theys)
// No AI was used in the creation of this file

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "pdollar.h"

// this library was imported from https://github.com/nlohmann/json
// for the purposes of saving the state of the recognizer between runs of the program
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

void printHelpMessage();
void addGestureTemplate(string filename);
void recognizeGestures(string filename);
int loadRecognizerState();
int saveRecognizerState();

static PDollarRecognizer recognizer = PDollarRecognizer();
static const string recognizerStatePath = "recognizerState.json";
static const string recognizerTemplatePath = "recognizerStateEmpty.json";

int main(int argc, char** args) {

    //int result;
    int result = loadRecognizerState();
    if (result < 0) {
        return -1;
    }

    if (argc <= 1) {
        printHelpMessage();
    } else {
        string flag = args[1];

        if (flag == "-t") {
            addGestureTemplate(args[2]);
        }
        else if (flag == "-r") {
            recognizer.DeleteUserGestures();
            
        }
        else {
            recognizeGestures(args[1]);
        }

        result = saveRecognizerState();
        if (result < 0) {
            return -1;
        }

    }

    return 0;
} // end main()


int loadRecognizerState() {
    if (!filesystem::exists(recognizerStatePath)) {
        cout << "Error: could not find " << recognizerStatePath << endl;
        return -1;
    }

    ifstream file;
    file.open(recognizerStatePath);

    if (!file.is_open()) {
        cout << "Could not open file: " << recognizerStatePath << endl;
        return -1;
    }

    json input(json::value_t::object);

    file >> input;

    file.close();

    json cloudTemplateList(json::value_t::array);

    cloudTemplateList = input["templates"];
    int numTemplates = input["num-templates"].template get<int>();


    for (int i = 0; i < numTemplates; i++) {

        json currTemplate = cloudTemplateList[i];

        string name = currTemplate["name"].template get<std::string>();
        vector<Point> pointsVector = {};

        // cout << name << endl;
        // cout << currTemplate << endl;

        json jCloudPoints = currTemplate["points"];
        
        for (int j = 0; j < jCloudPoints.size(); j++) {
            json currPoint = jCloudPoints[j];

            // cout << currPoint << endl;

            Point temp = Point(currPoint["X"].template get<double>(), currPoint["Y"].template get<double>(), currPoint["ID"].template get<int>());

            pointsVector.push_back(temp);

        }

        recognizer.AddGesture(name, pointsVector);
    }

    return 0;
} // end loadRecognizerState()


int saveRecognizerState() {
    if (!filesystem::exists(recognizerStatePath)) {
        cout << "Error: could not find " << recognizerStatePath << endl;
        return -1;
    }

    json cloudTemplateList(json::value_t::array);
    int countTemplates = 0;

    for (int i = 0; i < recognizer.size(); i++) {
        PointCloud currCloud = recognizer.pointClouds.at(i);

        string name = currCloud.name;
        vector<Point> currPoints = currCloud.points;

        json jCloudPoints(json::value_t::array);
        
        for (int j = 0; j < currPoints.size(); j++) {
            Point currPoint = currPoints.at(j);
            json p;
            p["X"] = currPoint.X;
            p["Y"] = currPoint.Y;
            p["ID"] = currPoint.strokeID;

            jCloudPoints.insert(jCloudPoints.end(), p);

        }

        json pointCloudObject = {
            {"name", name}, 
            {"points", jCloudPoints},
            {"num-points", currPoints.size()}
        };

        cloudTemplateList.insert(cloudTemplateList.end(), pointCloudObject);
        countTemplates++;

    }

    // cout << setw(4) << cloudTemplateList << endl;

    ofstream file;
    file.open(recognizerStatePath);

    if (!file.is_open()) {
        cout << "Could not open file: " << recognizerStatePath << endl;
        return -1;
    }

    json output = {
        {"num-templates", countTemplates},
        {"templates", cloudTemplateList}
    };

    file << setw(4) << output << endl;

    file.close();

    return 0;

} // end saveRecognizerState()


void addGestureTemplate(string filename) {
    // open file
    ifstream file;
    file.open(filename, ios::in);

    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return;
    }

    string gesturename = "";
    vector<Point> points = {};
    int strokeId = 1;

    // get name of gesture
    if (!file.eof()) {
        getline(file, gesturename);
        int len = gesturename.length();
        if (gesturename.at(len-1) == '\r') {
            gesturename = gesturename.substr(0, gesturename.length()-1);
        }
    }

    // while loop, add points to vector
    while(!file.eof()) { 
        string line; 
        getline(file, line); 

        int len = line.length();
        if (len == 0) {
            continue;
        }

        if (line.at(len-1) == '\r') {
            line = line.substr(0, line.length()-1);
        }

        if (strcmp(line.data(), "BEGIN") == 0) {
            continue;
        } else if (strcmp(line.data(), "END") == 0) {
            strokeId++;
            continue;
        }

        int x;
        int y;

        string stringX = strtok(line.data(), ",");
        string stringY = strtok(NULL, ",");

        x = stoi(stringX.data());
        y = stoi(stringY.data());

        // cout << "x: " << x << endl;
        // cout << "y: " << y << endl;

        Point temp = Point(x, y, strokeId);
        points.push_back(temp);
    } 

    // at end of file, add pointcloud to big gesture array
    int num = recognizer.AddGesture(gesturename, points);
    // num is the number of templates for this gesture, idk what we want to do with that 

    // close file

    file.close();
} // end addGestureTemplate()


void recognizeGestures(string filename) {
    // open file
    ifstream file;
    file.open(filename, ios::in);

    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return;
    }
    
    vector<Point> points = {};
    vector<Result> results = {};
    int strokeId = 0;

    // while loop, add points to vector
    while(!file.eof()) { 
        string line; 
        getline(file, line); 

        int len = line.length();
        if (len == 0) {
            continue;
        }

        if (line.at(len-1) == '\r') {
            line.at(len-1) = '\0';
        }

        if (strcmp(line.data(), "MOUSEDOWN") == 0) {
            continue;
        } else if (strcmp(line.data(), "MOUSEUP") == 0) {
            strokeId++;
            continue;
        } else if (strcmp(line.data(), "RECOGNIZE") == 0) {
            // call Recognize()
            Result temp = recognizer.Recognize(points);
            // clear points
            points.clear();
            // add result to vector of results
            results.push_back(temp);
            
            continue;
        }

        int x;
        int y;

        string stringX = strtok(line.data(), ",");
        string stringY = strtok(NULL, ",");

        x = stoi(stringX.data());
        y = stoi(stringY.data());

        // cout << "x: " << x << endl;
        // cout << "y: " << y << endl;

        Point temp = Point(x, y, strokeId);
        points.push_back(temp);
    }

    // close file
    file.close();

    // print out recognized gestures
    for (int i = 0; i < results.size(); i++) {
        cout << results[i].name << endl;
    }

} // end recognizeGestures()


void printHelpMessage() {
    cout << "Available commands:" << endl;
    cout << endl;

    cout << "./pdollar –t <gesturefile>" << endl;
    cout << "\tAdds the gesture file to the list of gesture templates." << endl;
    cout << endl;

    cout << "./pdollar -r" << endl;
    cout << "\tClears the templates." << endl;
    cout << endl;

    cout << "./pdollar <eventstream>" << endl;
    cout << "\tPrints the name of gestures as they are recognized from the event stream." << endl;
    cout << endl;
} // end printHelpMessage()