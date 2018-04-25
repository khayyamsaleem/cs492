/*
 * playground.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: khayyamsaleem
 */
#include <file.h>
#include <hdt.h>
#include <ldisk.h>
#include <lfile.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>


int main(int argc, char* argv[]) {
	std::string info_on_files, info_on_directories;
	unsigned disk_size, block_size;

	/* argument parsing */
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'f':
				info_on_files = argv[++i];
				break;
			case 'd':
				info_on_directories = argv[++i];
				break;
			case 's':
				disk_size = std::atoi(argv[++i]);
				break;
			case 'b':
				block_size = std::atoi(argv[++i]);
				break;
			default:
				std::cerr << "FLAG " << argv[i][1] << " NOT VALID!" << std::endl;
				std::exit(1);
				break;
			}
		} else {
			std::cerr << "INVALID USAGE!" << std::endl;
			std::exit(1);
		}
	}

	/* initial time stamp */
	auto init_t = std::chrono::system_clock::now();

	/* initialize global hierarchical directory tree */
	hdt *G = nullptr;

	/* initialize list of contiguous disk blocks */
	ldisk *block_sets = new ldisk();
	block_sets->add(new disk_node(0,ceil(disk_size/(float)block_size)-1,false,nullptr));

	/* get info out of dir_list */
	std::ifstream f_dirs(info_on_directories);
	std::string line;
	std::vector<std::string> directories;
	unsigned lc = 0;
	if (f_dirs.good()) {
		while (std::getline(f_dirs, line)) {
			if (line.empty())
				continue;
			if (lc == 0) {
				/* create root */
				File *f = new File(0, line.substr(0, line.find_last_of("/")),
						init_t, DIR_T);
				G = new hdt(f);
			} else {
				/* add directory to tree */
				File *f = new File(0, line, init_t, DIR_T);
				G->insert_node(f);
			}
			lc++;
		}
	} else {
		std::cerr << "Invalid file for dir list." << std::endl;
		std::exit(1);
	}

	/* get info out of file_list */
	std::ifstream f_files(info_on_files);
	std::vector<File> files;
	if (f_files.good()) {
		while (std::getline(f_files, line)) {
			if (line.empty())
				continue;
			std::istringstream is(line);
			double size_in_bytes;
			std::string f_path, garb;
			if (is >> garb >> garb >> garb >> garb >> garb >> garb
					>> size_in_bytes >> garb >> garb >> garb >> f_path) {
				/* add file to tree */
				File *f = new File(size_in_bytes, f_path, init_t, FILE_T, block_size);
				G->insert_node(f);
				std::cout << "After inserting \"" << f_path << "\" of size " << size_in_bytes << ": " << *block_sets << std::endl;
			} else {
				std::cerr << "Unexpected format for file list." << std::endl;
				std::exit(1);
			}
		}
	} else {
		std::cerr << "Invalid file for file list." << std::endl;
		std::exit(1);
	}

//	std::cout << *G << std::endl;
	std::cout << *block_sets << std::endl;



















	std::exit(0);
}

