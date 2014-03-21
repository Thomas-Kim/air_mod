#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

#define STR_LENGTH strlen("Regular File: ")

#define JPEG "jpeg"
#define PNG "png"
#define BMP "bitmap"

namespace fs = boost::filesystem;
namespace sys = boost::system;

std::string is_image(fs::path p);
std::vector<fs::path> get_images(fs::path directory);

class image_folder {
    public:
        image_folder(std::string path);
        image_folder(fs::path path);
        void get_new_images();
        const std::vector<fs::path> & get_image_vector(){ return image_vector;          }
        std::vector<fs::path>::const_iterator begin() {   return image_vector.begin();  }
        std::vector<fs::path>::const_iterator end() {     return image_vector.end();    }
        void clear_images() { image_vector.clear(); }
    private:
        std::vector<fs::path> image_vector;
        fs::path directory;
        std::time_t last_checked;
};
#endif
