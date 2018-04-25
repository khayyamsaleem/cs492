#include <file.h>
#include <hdt.h>
#include <ldisk.h>
#include <lfile.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>


/* displays valid commands */
const char* valid_commands(){
	 return "Valid commands:\n"
	 "\tcd [directory] - set specified directory as the current directory\n"
	 "\tcd.. - set parent directory as current directory\n"
	 "\tls - list all files and sub-directories in current directory\n"
	 "\tmkdir [name] - create a new directory in the current directory\n"
	 "\tcreate [name] - create a new file in the current directory\n"
	 "\tappend [name] [bytes] - append a number of bytes to the file\n"
	 "\tremove [name] [bytes] - delete a number of bytes from the file\n"
	 "\tdelete [name] - delete the file or directory\n"
	 "\texit - deallocate data structures and exit program\n"
	 "\tdir - print out directory tree in breadth-first order\n"
	 "\tprfiles - print out all file information\n"
	 "\tprdisk - print out disk space information";
}

/* splits string by spaces */
std::vector<std::string> tokenize(std::string s, std::string delim){
	std::string buf;
	std::stringstream ss(s);
	std::vector<std::string> tokens;
	while (ss >> buf) if (!buf.empty()) tokens.push_back(buf);
	return tokens;
}

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
	block_sets->add(new disk_node(0,ceil(disk_size/(float)block_size) -1,false,nullptr));

//	std::cout << "here" << std::endl;

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
//				std::cout << "there" << std::endl;
//				std::cout << "line: " << line << std::endl;
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

//	std::cout << "here" << std::endl;
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
				File *f = new File(size_in_bytes, f_path, init_t, FILE_T);
				G->insert_node(f);
			} else {
				std::cerr << "Unexpected format for file list." << std::endl;
				std::exit(1);
			}
		}
	} else {
		std::cerr << "Invalid file for file list." << std::endl;
		std::exit(1);
	}

	std::cout << "FULL FILE TREE" << std::endl;
	std::cout << *G << std::endl;

	/* starting repl */
	hdt *cwd = G;
	std::cout << cwd->data->file_path << " >>> ";
	std::cout.flush();
	while (std::getline(std::cin, line)){
		std::vector<std::string> tokens = tokenize(line, " ");

		/* commands without arguments */
		if(tokens.size()==1){
			std::string cmd = tokens[0];


			/************ cd.. **********/
			if (cmd == "cd.."){
				if (cwd == G){
					std::cout << "Already at root!" << std::endl;
				} else {
					cwd = cwd->parent;
				}




			/************ ls **********/
			} else if (cmd == "ls"){
				for (auto it = cwd->children.begin(); it != cwd->children.end(); ++it)
					std::cout << *((*it)->data) << std::endl;



			/************ exit **********/
			} else if (cmd == "exit") {
				std::cout << "Bye!" << std::endl;
				delete G;
				delete block_sets;
				std::exit(0);



			/************ dir **********/
			} else if (cmd == "dir"){
				std::cout << *G << std::endl;



			/************ prdisk **********/
			} else if (cmd == "prdisk") {
				// TODO: print out all disk space information



			/************ prfiles **********/
			} else if (cmd == "prfiles") {
				//TODO: print out all file information
			} else {
				std::cerr << "Invalid command!" << std::endl;
				std::cout << valid_commands() << std::endl;
			}




		/* commands with one argument */
		} else if (tokens.size() == 2){
			std::string cmd = tokens[0];
			std::string arg = tokens[1];


			/************ cd **********/
			if (cmd == "cd"){
				/* adding this due to force of habit, don't @ me */
				if (arg == ".."){
					if (cwd == G){
						std::cout << "Already at root!" << std::endl;
					} else {
						cwd = cwd->parent;
					}
				} else {
					bool found = false;
					for(auto it = cwd->children.begin(); it != cwd->children.end(); ++it){
						if ((*it)->data->file_name == arg){
							found = true;
							if ((*it)->data->type==DIR_T){
								cwd = *it;
							} else {
								std::cerr << "Not a directory!" << std::endl;
							}
							break;
						}
					}
					if (!found)
						std::cerr << "Directory not found!" << std::endl;
				}



			/************ mkdir **********/
			} else if (cmd == "mkdir") {
				File *f = new File(0, cwd->data->file_path+"/"+arg, std::chrono::system_clock::now(), DIR_T);
				G->insert_node(f);




			/************ create **********/
			} else if (cmd == "create") {
				File *f = new File(0, cwd->data->file_path+"/"+arg, std::chrono::system_clock::now(), FILE_T);
				G->insert_node(f); //TODO: perform memory allocation
			} else if (cmd == "delete") {
				hdt* to_delete = G->find(cwd->data->file_path+"/"+arg);
				if (to_delete == nullptr){
					std::cerr << "No such file or directory" << std::endl;
				}
				if (to_delete->data->type == DIR_T && !to_delete->children.empty()){
					std::cerr << "Cannot delete non-empty directory!" << std::endl;
				} else {
					auto children = &(to_delete->parent->children);
					children->erase(std::remove_if(children->begin(), children->end(), [&](hdt* x){
						return x == to_delete;
					}));
					//TODO: perform memory deallocation on ldisk
				}
			} else {
				std::cerr << "Invalid command!" << std::endl;
				std::cout << valid_commands() << std::endl;
			}




		/* commands with two arguments */
		} else if (tokens.size() == 3) {
			std::string cmd = tokens[0];
			std::string fname = tokens[1];
			double bytes = std::stod(tokens[2]);
			(void)bytes; //silence "unused" warning for now



			/************ append **********/
			if (cmd == "append"){
				//TODO: append bytes to given file, make sure it's a file and not a directory





			/************ remove **********/
			} else if (cmd == "remove"){
				//TODO: remove bytes from given file, make sure it's a file and not a directory



			} else {
				std::cerr << "Invalid command!" << std::endl;
				std::cout << valid_commands() << std::endl;
			}


		/* skip if mans just passed a newline or something */
		} else if (line.empty() || tokens.empty()) {
			;

		/* commands can only by 1, 2, or 3 tokens long */
		} else {
			std::cerr << "Invalid command!" << std::endl;
			std::cout << valid_commands() << std::endl;
		}

		std::cout << cwd->data->file_path << " >>> ";
		std::cout.flush();
	}
}
