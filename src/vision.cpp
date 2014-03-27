#include <iostream>
#include <string>
#include <vector>
#include <map>

/* OPENCV HEADERS */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "opencv2/nonfree/features2d.hpp"

/* BOOST HEADERS */
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "directory.hpp"
#include "surf_map.hpp"
#include "stats.hpp"

/* ROS HEADERS */
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

// Being lazy :|
namespace po = boost::program_options;
using namespace std;
using namespace fs;
using namespace po;
using namespace cv;

typedef Mat Descriptors;
typedef vector<KeyPoint> KeyPoints;
typedef pair<string, Mat> MatPair;

image_transport::Subscriber img_sub;

surf_map *smap;

// put the detection stuff in here
void callback(const sensor_msgs::ImageConstPtr &msg){

    keypoints_t matched_keypoints;
    fs::path matched_path;
    naive_stats naive_model(training_path);
    fs::path current_image;
    keypoints_t current_keypoints;
    vector<DMatch> matches;
    Mat img_matches;

    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Access image by doing cv_ptr->image
    // Do it like this?
    // matches = smap->match(cv_ptr->image, matched_keypoints, matched_path, current_keypoints);

    //detection.get_new_images();
    //for(vector<path>::const_iterator itr = detection.begin(); itr != detection.end(); ++itr){
        // Need to do surf_map smap?
        //matches = smap.match(*itr, matched_keypoints, matched_path, current_keypoints);

	// smap.match(path img, keypoints_t &matched_keypoints, path &matched_path, keypoints_t &detection_keypoints);
	// want to change smap.match to accept an image instead

	// also!! want to have something that converts the ImageConstPtr to an image

}

int main(int argc, char * argv[]){
    path training_path, detection_path;
    int surf_threshold;
    /* Command line argument processing */
    options_description desc("Arguments");
    desc.add_options()
        ("help", "print help")
        ("training", value<string>(), "-t/--training <DIRECTORY>, pass in training image set")
        ("detection", value<string>(), "-d/--detection <DIRECTORY>, pass in testing image set") // remove later
        ("threshold", value<int>(), "-th/--threshold <INT>, pass in surf threshold")
        ;
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if(vm.count("help")){
        cout << desc << endl;
        return 0;
    }

    if(vm.count("training"))
        training_path = path(vm["training"].as<string>());
    else{
        cerr << "training image set not specified" << endl;
        cerr << "--training <TRAINING_SET_DIR>" << endl;
        return 1;
    }
    if(vm.count("detection")) // remove later
        detection_path = path(vm["detection"].as<string>());
    else{
        cerr << "testing image set not specified" << endl;
        cerr << "--detection <TESTING_SET_DIR>" << endl;
        return 1;
    }
    if(vm.count("threshold"))
        surf_threshold = vm["threshold"].as<int>();
    else
        surf_threshold = 2500;
    /* End of command line argument processing */

    /* get_images defined in directory.cpp */
    image_folder training(training_path);
    image_folder detection(detection_path); //

    /* Initialize main window and SURF */
    initModule_nonfree();
    namedWindow("Chicken", WINDOW_AUTOSIZE);

    smap = new surf_map(surf_threshold);
    
    training.get_new_images();
    for(vector<path>::const_iterator itr = training.begin(); itr != training.end(); ++itr){
      /* std::cout << *itr << std::endl; */
      smap.train(*itr);
    }
    smap.init();


    /*** Need to move this somewhere else? Ok. ***/
    keypoints_t matched_keypoints;
    /* descriptors_t matched_descriptors; */
    fs::path matched_path;
    naive_stats naive_model(training_path);
    fs::path current_image;
    keypoints_t current_keypoints;
    vector<DMatch> matches;
    Mat img_matches;

/* Move this portion over to callback since you're doing it real time ish */
/*    bool found = false;
    while(found == false) {
        detection.get_new_images();
        for(vector<path>::const_iterator itr = detection.begin(); itr != detection.end(); ++itr) {
            matches = smap.match(*itr, matched_keypoints, matched_path, current_keypoints);
            std::cout << *itr << " matched with " << matched_path << " and had a DMatch vector size of " << matches.size() << std::endl;
            naive_model.add_match(matched_path);
            current_image = *itr;
            drawMatches(imread(current_image.string()), current_keypoints, imread(matched_path.string()), matched_keypoints, matches, img_matches);
            imshow("Chicken", img_matches);
            char choice = '\0';
            while(choice != '\n')
                choice = waitKey(10);
            if(naive_model.get_match(0.95) != "") {
                std::cout << "Matched: " << naive_model.get_match(0.95) << std::endl;
                found = true;
                break;
            }
        }
        detection.clear_images();
    }
*/

    /* Set up for ROS */
    ros::init(argc, argv, "_surf_matcher");
    ros::NodeHandle n;

    image_transport::ImageTransport it(n);
    img_sub = it.subscribe("camera/rgb/image_color", 1, callback);
    
    ros::spin();

    return 0;

}
