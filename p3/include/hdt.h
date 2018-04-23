#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <vector>

struct File {
    double size;
    std::string file_path;
    std::chrono::system_clock::time_point time_stamp;

    File(double size, std::string file_path, std::chrono::system_clock::time_point time_stamp){
        this->size = size;
        this->file_path = file_path;
        this->time_stamp = time_stamp;
    }

    double get_size(){
        return this->size;
    }

    std::string get_file_path(){
        return this->file_path;
    }

    std::chrono::system_clock::time_point get_time_stamp(){
        return this->time_stamp;
    }

    friend std::ostream& operator<<(std::ostream& os, File &f){
        std::time_t t =  std::chrono::system_clock::to_time_t(f.get_time_stamp());
        std::string time_string = std::ctime(&t);
        time_string = time_string.substr(0, time_string.length()-1);
        os << "File [ path=" << f.get_file_path() <<
                    " size=" << f.get_size() <<
                    " timestamp=" << time_string  << " ]";
        return os;
    }
};

struct hdt_node {
    File data;
    std::vector<hdt_node> children;
}
