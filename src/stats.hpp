#ifndef STATS_HPP
#define STATS_HPP
#include <iostream>
#include <boost/filesystem.hpp>
#include <map>

#define INITIAL_VALUE 1

// define a sufficiently large number of matches (IE: 1 match will be 100% but still rejected if LARGE_THRESHOLD > 1)
#define LARGE_THRESHOLD 15
namespace fs = boost::filesystem;

class detection_stats {
    public:
        detection_stats(fs::path parent_directory);
        /* add a matched path to the model */
        virtual void add_match(fs::path match);
        /* query the model for the current most likely match over some confidence interval
         * returns NULL if a match cannot be determined */
        virtual fs::path get_match(double confidence);

    protected:
        std::map<fs::path, int> model;
};

class naive_stats : public detection_stats {
    public:
        naive_stats(fs::path parent_directory) : detection_stats(parent_directory) {}
        void add_match(fs::path match) { detection_stats::add_match(match); }
        fs::path get_match(double confidence) { return detection_stats::get_match(confidence); }
};

class decay_stats : public detection_stats {
    public:
        decay_stats(fs::path parent_directory) : detection_stats(parent_directory) {}
        void add_match(fs::path match);
        fs::path get_match(double confidence) { return detection_stats::get_match(confidence); }
};
#endif
