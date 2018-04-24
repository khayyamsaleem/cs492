#pragma once

#include <sys/_types/_time_t.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

/* since files and directories are both files */
enum kind {
	DIR_T, FILE_T
};

/* represents a single file or directory */
struct File {
	double size; /* size in bytes */
	std::string file_path; /* full file path */
	std::chrono::system_clock::time_point time_stamp; /* last modified */
	kind type; /* FILE_T or DIR_T */

	/* constructor for a file */
	File(double size, std::string file_path,
			std::chrono::system_clock::time_point time_stamp, kind type) {
		this->size = size;
		this->file_path = file_path;
		this->time_stamp = time_stamp;
		this->type = type;
	}

	/* returns file size in bytes */
	double get_size() {
		return this->size;
	}

	/* returns path */
	std::string get_path() {
		return this->file_path;
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
		os << " [ size=" << f.get_size() << "; path=\"" << f.get_path()
				<< "\" ]";
		return os;
	}

};

