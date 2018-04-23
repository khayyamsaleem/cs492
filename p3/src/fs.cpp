#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include "hdt.h"
#include "ldisk.h"

int main(int argc, char* argv[]){
    std::string info_on_files, info_on_directories;
    unsigned disk_size, block_size;

    /* argument parsing */
    for(int i = 1; i < argc; ++i){
        if (argv[i][0] == '-'){
            switch (argv[i][1]) {
                case 'f': info_on_files = argv[++i];
                          break;
                case 'd': info_on_directories = argv[++i];
                          break;
                case 's': disk_size = std::atoi(argv[++i]);
                          break;
                case 'b': block_size = std::atoi(argv[++i]);
                          break;
                default:  std::cerr << "FLAG " << argv[i][1] << "NOT VALID!" << std::endl;
                          std::exit(1);
                          break;
            }
        } else {
            std::cerr << "INVALID USAGE!" << std::endl;
            std::exit(1);
        }
    }

    /* get info out of file_list */
    std::string line;
    std::ifstream f_files(info_on_files);
    std::vector<File> files;
    if (f_files.good()){
        while (std::getline(f_files, line)){
            if (line.empty()) continue;
            std::istringstream is(line);
            double size_in_bytes;
            std::string f_path, garb, m, d, t;
            std::stringstream ts;
            if (is >> garb >> garb >> garb >> garb >> garb >> garb >>
                    size_in_bytes >> m >> d >> t >> f_path){
                ts << m << " " << d << " " << t;
                // std::cout << "size in bytes: " << size_in_bytes << std::endl;
                // std::cout << "file path: " << f_path << std::endl;
                // std::cout << "time stamp: " << ts.str() << std::endl;
                File f{size_in_bytes, f_path, std::chrono::system_clock::now()};
                files.push_back(f);
            } else {
                std::cerr << "Unexpected format for file list." << std::endl;
                std::exit(1);
            }
        }
    } else {
        std::cerr << "Invalid file for file list." << std::endl;
        std::exit(1);
    }
    for (auto it = files.begin(); it != files.end(); ++it)
      std::cout << *it << std::endl;

    /* get info out of dir_list */
    std::ifstream f_dirs(info_on_directories);
    std::vector<std::string> directories;
    if(f_dirs.good()){
        while(std::getline(f_dirs, line)){
            if (line.empty()) continue;
            directories.push_back(line);
        }
    } else {
        std::cerr << "Invalid file for dir list." << std::endl;
        std::exit(1);
    }

    /* testing ldisk behavior */
    ldisk l;
    disk_node d {1, 5, true};
    l.add(d);
    disk_node f {6, 10, false};
    l.add(f);
    disk_node h {11, 16, false};
    l.add(h);
    disk_node i {17, 23, false};
    l.add(i);
    disk_node k {24, 32, true};
    l.add(k);
    std::cout << l << std::endl;
    l.merge();
    std::cout << l << std::endl;


    std::exit(0);
}
