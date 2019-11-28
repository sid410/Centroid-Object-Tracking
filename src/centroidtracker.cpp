/*
Created by pratheek on 2019-11-27.
*/
#include "centroidtracker.h"

using namespace std;

CentroidTracker::CentroidTracker(int maxDisappeared) {
    this->nextObjectID = 0;

    // ordered dicts "objects" and "disappeared"
    //    disappeared.insert(pair<int, int>(5, 4));

    this->maxDisappeared = maxDisappeared;
}


void CentroidTracker::register_Object(int cX, int cY) {
//    cout << "Centroids: " << cX << " " << cY << endl;
    int object_ID = this->nextObjectID;
    this->objects.insert(objects.end(), {object_ID, {cX, cY}});
    this->disappeared.insert(disappeared.end(), {object_ID, 0});
    this->nextObjectID += 1;

    for(auto elem : this->objects)
    {
        std::cout << "CHECKING OBJECTS: " << elem.first << " " << elem.second.first << " " << elem.second.second << "\n";
    }
}

void CentroidTracker::deregister_Object(int objectID) {
    cout << "Deregistered object: " << objectID << endl;
    if (!this->objects.empty()){
        this->objects.erase(objectID);
        this->disappeared.erase(objectID);
    }
}

std::map<int, std::pair<int, int>> CentroidTracker::update(vector<vector<int>> boxes) {

    if (boxes.empty()) {
//        cout << "NO BOXES BROOOO" << endl;

        if (!disappeared.empty()) {
            for(auto elem : this->disappeared)
            {
//                std::cout << "CHECKING DISAPPEARED: " << elem.first << " " << elem.second << " " << disappeared.size() << endl;
                disappeared[elem.first] ++;

                if (elem.second > this->maxDisappeared){
                    this->deregister_Object(elem.first);
                }
            }
        }
        return this->objects;
    }
    // initialize an array of input centroids for the current frame
    vector<vector<int>> inputCentroids;
    inputCentroids.clear();
//    for (auto i : boxes){
//        inputCentroids.insert(inputCentroids.end(), {0,0});
//        cout << inputCentroids.front().size() << endl;
//    }

    // loop over the bounding box rectangles
    for (auto i : boxes) {
        // use the bounding box coordinates to derive the centroid
        int cX = int((i[0] + i[2]) / 2.0);
        int cY = int((i[1] + i[3]) / 2.0);
        inputCentroids.insert(inputCentroids.end(), {cX, cY});
    }
//    cout << "SIZE = " << inputCentroids.size() << endl;

    //if we are currently not tracking any objects take the input centroids and register each of them
    if (this->objects.empty()) {
        for (auto &row:inputCentroids) {
            this->register_Object(row[0], row[1]);
        }
    }

//    for(auto elem : objects)
//    {
//        std::cout << "CHECKING OBJECTS: " << elem.first << " " << elem.second.first << " " << elem.second.second << "\n";
//    }

    return this->objects;
}
