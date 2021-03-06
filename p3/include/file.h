#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <lfile.h>
#include <colors.h>
#include <vector>

/* since files and directories are both files */
enum kind {
	DIR_T, FILE_T
};


/* represents a single file or directory */
struct File {
	double size; /* size in bytes */
	std::string file_path; /* full file path */
	std::string file_name; /*just name*/
	std::chrono::system_clock::time_point time_stamp; /* last modified */
	kind type; /* FILE_T or DIR_T */
	unsigned remaining = 0; /* amount in last block */
//	lfile* lf;
	std::vector<unsigned> lf;

	/* constructor for a file */
	File(double size, std::string file_path,
			std::chrono::system_clock::time_point time_stamp, kind type) {
		this->size = size;
		this->file_path = file_path;
		this->time_stamp = time_stamp;
		this->type = type;
		if (file_path.find_last_of("/") == std::string::npos)
			this->file_name = file_path;
		else
			this->file_name = file_path.substr(file_path.find_last_of("/")+1, std::string::npos);
	}

	/* returns file size in bytes */
	double get_size() {
		return this->size;
	}

	/* returns full path */
	std::string get_path() {
		return this->file_path;
	}

	std::string get_name(){
		return this->file_name;
	}

	/* returns time stamp */
	std::chrono::system_clock::time_point get_time_stamp() {
		return this->time_stamp;
	}


	/* used to print a file */
	friend std::ostream& operator<<(std::ostream& os, File &f) {
		std::time_t t = std::chrono::system_clock::to_time_t(
				f.get_time_stamp());
		std::string time_string = std::ctime(&t);
		time_string = time_string.substr(0, time_string.length() - 1);
		if (f.type == DIR_T){
			os << blue << bold_on << "DIR " << bold_off << blue;
			os << " [ "<<
					bold_on << "name=\"" << f.get_name() <<  "\"" << bold_off << blue <<"; " <<
					"full_path=\"" << f.get_path() << "\"" <<
					" ]";
			os << def;
		} else {
			os << green << bold_on << "FILE" << bold_off << green;
			os << " [ "<<
					bold_on << "name=\"" << f.get_name() << "\"" << bold_off << green << "; " <<
					"size=" << f.get_size() << "; "  <<
					"full_path=\"" << f.get_path() << "\"; " <<
					"timestamp="<<time_string << " ]";
			os << def;
		}
		return os;
	}

};

