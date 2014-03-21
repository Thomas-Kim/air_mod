#include "surf_map.hpp"

surf_map::surf_map(int threshold){
    surf = SURF(threshold);
    initModule_nonfree();
}

surf_map::~surf_map(){
    // TODO: check how opencv containers deallocate
}

void surf_map::train(path img) {
    // TODO: add error checking / exception handling

    descriptors_t img_descriptor;
    keypoints_t  img_keypoint;

    surf(imread(img.string()), noArray(), img_keypoint, img_descriptor);

    database[img_descriptor] = pair<keypoints_t, path>(img_keypoint, img);
    training_descriptors.push_back(img_descriptor);
}

vector<DMatch> & surf_map::match(path img, keypoints_t & matched_keypoints, path & matched_path, keypoints_t & detection_keypoints) {

    // TODO: add error checking / exception handling
    descriptors_t detection_descriptors;

    surf(imread(img.string()), noArray(), detection_keypoints, detection_descriptors);
    // match(const Mat& queryDecriptors, vector<DMatch>& matches)
    matcher.match(detection_descriptors, matches);

    // DMatch contains queryIdx, trainIdx, and imgIdx
    // Want to iterate through matches
    matched_path = database[training_descriptors.at(matches.begin()->imgIdx)].second;
    matched_keypoints = database[training_descriptors.at(matches.begin()->imgIdx)].first;
    int count = 0;

    for(vector<DMatch>::const_iterator itr = matches.begin(); itr != matches.end(); itr++){
        count++;
        std::cout << "queryIdx: " << std::setw(3) << (*itr).queryIdx <<
        " | trainIdx: " << std::setw(3) << (*itr).trainIdx <<
        " | imgIdx: " << (*itr).imgIdx << std::endl;
    }


    return matches;
}
