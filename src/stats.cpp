#include "stats.hpp"

detection_stats::detection_stats(fs::path parent_directory) {
      for (fs::recursive_directory_iterator iter(parent_directory), end; iter != end; ++iter)
          if(fs::is_directory(iter->path()))
             model[iter->path()] = INITIAL_VALUE;
}

void detection_stats::add_match(fs::path match) {
    fs::path match_folder = match.parent_path();
    if(model.count(match_folder))
        model[match_folder] += 1;
    else
        model[match_folder] = 0;
}

fs::path detection_stats::get_match(double confidence) {
    unsigned long long total = 0;
    unsigned long long max = 0;
    fs::path match;
    for(std::map<fs::path, int>::iterator itr = model.begin(); itr != model.end(); itr++) {
        total += model[itr->first];
        if(model[itr->first] > max) {
            match = itr->first;
            max = model[itr->first];
        }
    }
    if(total < LARGE_THRESHOLD)
        return fs::path("");
    if((double)max / total >= confidence)
        return match;
    else
        return fs::path("");
}

void decay_stats::add_match(fs::path match) {
    /* Decay */
    fs::path match_folder = match.parent_path();
    for(std::map<fs::path, int>::iterator itr = model.begin(); itr != model.end(); ++itr)
        if(itr->first != match_folder)
            --(itr->second);
    /* Add */
    detection_stats::add_match(match);
}
