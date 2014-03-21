#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

/* OPENCV HEADERS */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "opencv2/nonfree/features2d.hpp"

#include "directory.hpp"

using namespace std;
using namespace fs;
using namespace cv;

typedef Mat descriptors_t;
typedef vector<KeyPoint> keypoints_t;
typedef pair<keypoints_t, path> keypath_t;

class descriptor_comp{
  public:
    bool operator() (const descriptors_t& lhs, const descriptors_t& rhs){
        // larger size matrix is larger
        // otherwise compare by element and first to have a larger element is the larger... row by row
      // 1. Overall size
      if(lhs.total()!=rhs.total()) return (lhs.total() < rhs.total());
      // 2. Rows
      if(lhs.rows!=rhs.rows)       return (lhs.rows < rhs.rows);
      // 3. Columns
      if(lhs.cols!=rhs.cols)       return (lhs.cols < rhs.cols);

      // 4. Memberwise
      for(int i = 0; i < lhs.rows; i++)
      {
        for(int j = 0; j < lhs.cols; j++)
        {
          if(lhs.at<double>(i,j)!= rhs.at<double>(i,j)) return (lhs.at<double>(i,j) < rhs.at<double>(i,j));
        }
      }

      // If lhs==rhs, then default to rhs < lhs
      return false;
    }

};

class surf_map {
  public:
      surf_map(int threshold = 2500);
      ~surf_map();
      void train(path img);
      void init() { matcher.add(training_descriptors); }
      // TODO: want match to return whatever's necessary to draw?
      // soooo what I'm thinking is that it'll need to return keypoints, descriptors, and image name
      // please implement the function using the following header:
      vector<DMatch> & match(path img, keypoints_t & matched_keypoints, path & matched_path, keypoints_t & detection_keypoints);

  private:
      /* BFMatcher             matcher; */
      vector<descriptors_t> training_descriptors;
      FlannBasedMatcher     matcher;
      vector<DMatch>        matches;
      SURF                  surf;
      // TODO: Make data structure that will hold keypoints and path to images
      map<descriptors_t, keypath_t, descriptor_comp> database;
};
