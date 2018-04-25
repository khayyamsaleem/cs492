#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <lfile.h>

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
//	unsigned alloc = 0;
	lfile* lf;

	/* constructor for a file */
	File(double size, std::string file_path,
			std::chrono::system_clock::time_point time_stamp, kind type, double block_size=-1) {
		this->size = size;
		this->file_path = file_path;
		this->time_stamp = time_stamp;
		this->type = type;
		if(type==FILE_T) this->lf = new lfile(block_size);
		else this->lf = nullptr;
		if (file_path.find_last_of("/") == std::string::npos)
			this->file_name = file_path;
		else
			this->file_name = file_path.substr(file_path.find_last_of("/"), std::string::npos);
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
		if (f.type == DIR_T)
			os << "DIR ";
		else
			os << "FILE";
		os << " [ size=" << f.get_size() << ";" << std::endl <<
				"\t       path=\"" << f.get_path() << "\";" << std::endl <<
				"\t       name=\""<< f.get_name() <<"\"" << std::endl <<
				"\t     ]";
		return os;
	}

};

