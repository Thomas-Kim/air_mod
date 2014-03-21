#include "directory.hpp"

// So we don't get yucky file types
std::string is_image(fs::path p){
    if(p.has_extension() == false)
        return "";
    if(p.extension() == ".jpg")
        return JPEG;
    if(p.extension() == ".png")
        return PNG;
    if(p.extension() == ".bmp")
        return BMP;
    return "";
}

std::vector<fs::path> get_images(fs::path directory){
    std::vector<fs::path> img_list;
    try{
        fs::path p(directory);
        if(!fs::exists(p) || !fs::is_directory(p)){
            std::cerr << "Not a directory" << std::endl;
            img_list;
        }

        fs::directory_iterator end;
        std::string type;
        for(fs::directory_iterator itr = fs::directory_iterator(p); itr != end; itr++){
           type = is_image((*itr).path());
            if(type != "")
                img_list.push_back((*itr).path());
        }
    }
    catch(std::exception e){
        std::cerr << e.what() << std::endl;
    }
    return img_list;
}

image_folder::image_folder(std::string path) : directory(path), last_checked(0x0), image_vector(std::vector<fs::path>()) {};
image_folder::image_folder(fs::path path) : directory(path), last_checked(0x0), image_vector(std::vector<fs::path>()) {};

void image_folder::get_new_images() {
    fs::directory_iterator end;
    std::string type;
    std::time_t latest_change = last_checked;
    for(fs::directory_iterator itr = fs::directory_iterator(directory); itr != end; itr++) {
        type = is_image((*itr).path());
        std::time_t changed = fs::last_write_time((*itr).path());
        if(type != "" && changed > last_checked) {
            image_vector.push_back((*itr).path());
            latest_change = changed > latest_change ? changed : latest_change;
        }
    }
    last_checked = latest_change;
    /* std::cout << std::ctime(&last_checked) << std::endl; */
}
