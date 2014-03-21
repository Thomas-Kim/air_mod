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

    std::map<int, int> match_filter;
    for(vector<DMatch>::const_iterator itr = matches.begin(); itr != matches.end(); itr++){
            match_filter[itr->imgIdx] += 1;
    }

    /* remove bad matches */
    int max_value = -1;
    int max_idx = -1;
    for(std::map<int, int>::iterator it = match_filter.begin(); it != match_filter.end(); ++it) {
        if(it->second > max_value) {
            max_value = it->second;
            max_idx = it->first;
        }
    }

    vector<DMatch> good_matches;

    for(vector<DMatch>::const_iterator it = matches.begin(); it != matches.end(); it++)
        if(it->imgIdx == max_idx) {
            std::cout << "queryIdx: " << std::setw(3) << (*it).queryIdx <<
                " | trainIdx: " << std::setw(3) << (*it).trainIdx <<
                " | imgIdx: " << std::setw(3) << (*it).imgIdx <<
                " | distance: " << std::setw(3) << (*it).distance << std::endl;
            good_matches.push_back(*it);
        }

    /* TODO fix this */
    matches = good_matches;
    matched_path = database[training_descriptors.at(matches.begin()->imgIdx)].second;
    matched_keypoints = database[training_descriptors.at(matches.begin()->imgIdx)].first;

    return matches;
}
