#include <file.h>
#include <hdt.h>
#include <l_disk.h>
//#include <lfile.h>
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
	l_disk *block_sets = new l_disk(disk_size, block_size);


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
				File *f = new File(0, line.substr(0, line.find_last_of("/")), init_t, DIR_T);
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

				//memory allocation work on ldisk for file
				unsigned blocks_required = ceil((1.0*size_in_bytes) / block_size);
				unsigned cur_block = 0;
				std::vector<unsigned> reserved;
				bool can_add;
				while(cur_block != block_sets->num_blocks && blocks_required != 0){
					if(!block_sets->blocks.at(cur_block)){
						block_sets->blocks.at(cur_block) = true;
						reserved.push_back(cur_block);
						--blocks_required;
					}
					cur_block++;
				}
				can_add = true;
				if (blocks_required){
					can_add = false;
					std::cerr << "NOT ENOUGH SPACE FOR FILE. NOT ADDED." << std::endl;
					for (unsigned b : reserved) block_sets->blocks.at(b)= false;
					reserved.clear();
				}
				if(can_add){ //only add to tree if there was enough space
					File *f = new File(size_in_bytes, f_path, init_t, FILE_T);
					f->lf = reserved;
					f->remaining = (int)size_in_bytes % (int)block_size;
					G->insert_node(f);
				}
			} else {
				std::cerr << "Unexpected format for file list." << std::endl;
				std::exit(1);
			}
		}
	} else {
		std::cerr << "Invalid file for file list." << std::endl;
		std::exit(1);
	}


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
				std::cout << *block_sets << std::endl;
				std::cout << "FRAGMENTATION: ";
				unsigned frag = 0;
				hdt *cur = G;
				std::queue<hdt*> q;
				while (cur) {
					if(cur->data->type == FILE_T){
						if (cur->data->remaining){
							frag += block_size - cur->data->remaining;
						}
					}
					for (auto it = cur->children.begin(); it != cur->children.end(); ++it)
						q.push(*it);
					if (q.empty()) cur = nullptr;
					else {
						cur = q.front();
						q.pop();
					}
				}
				std::cout << frag << " BYTES" << std::endl;


			/************ prfiles **********/
			} else if (cmd == "prfiles") {
				// find all files in the file tree
				//print name, size, ts
				// for each file, iterate over its lfile to yield block addresses
				hdt *cur = G;
				std::queue<hdt*> q;
				while (cur) {
					if(cur->data->type == FILE_T){
						std::cout << *(cur->data) << std::endl;
						//TODO: print out LFILE properly
						std::cout << "LFILE: ";
						if (cur->data->lf.size()){
							int start = cur->data->lf.at(0);
							for(auto it = cur->data->lf.begin(); it != cur->data->lf.end(); ++it){
								int block = *it;
								if (it == cur->data->lf.end()-1){
									if (block == start){
										std::cout << "(" << start*block_size << " -> " <<
												(start*block_size) + block_size << ") ";
									} else {
										std::cout << "(" << start*block_size << " -> " <<
												(block*block_size) + block_size << ") ";
									}
								} else if (*(it+1)-block != 1){
									if (block == start){
										std::cout << "("<< start*block_size << " -> " <<
												(start*block_size) + block_size << ") ";
									} else {
										std::cout << "("<< start*block_size << " -> " <<
												(block*block_size) + block_size << ") ";
									}
									start = *(it+1);
								}
							}
							std::cout << std::endl;
						} else {
							std::cout << "(no blocks allocated)" << std::endl;
						}
					}
					for (auto it = cur->children.begin(); it != cur->children.end(); ++it)
						q.push(*it);
					if (q.empty()) cur = nullptr;
					else {
						cur = q.front();
						q.pop();
					}
				}


			/************ clear, for convenience **********/
			} else if (cmd == "clear") {
				std::system("clear");


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
				G->insert_node(f);


			/************ delete **********/
			} else if (cmd == "delete") {
				hdt* to_delete = G->find(cwd->data->file_path+"/"+arg);
				to_delete->parent->data->time_stamp = std::chrono::system_clock::now();
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

			//check if the file exists in current directory
			bool exists = false;
			File* to_mod;
			for(auto it = cwd->children.begin(); it != cwd->children.end(); ++it){
				if ((*it)->data->file_name == fname){
					exists = true;
					to_mod = (*it)->data;
					break;
				}
			}
			if (!exists){
				std::cerr << "NO SUCH FILE." << std::endl;
				delete to_mod;
			} else {

				/************ append **********/
				if (cmd == "append"){
					//TODO: append bytes to given file, make sure it's a file and not a directory
					to_mod->time_stamp = std::chrono::system_clock::now();
					unsigned remaining = to_mod->remaining;
					unsigned overflow = (int)bytes % (int)block_size;
					unsigned additional_blocks = 0;
					if(to_mod->remaining){
						unsigned excess = overflow + to_mod->remaining;
						if (excess < block_size){
							to_mod->remaining = excess;
						} else if (excess == block_size) {
							to_mod->remaining = 0;
						} else {
							additional_blocks += 1;
							to_mod->remaining = excess % block_size;
						}
					} else {
						to_mod->remaining = overflow;
					}
					if (bytes < block_size){
						additional_blocks +=1;
					} else {
						additional_blocks += ceil((bytes*1.0)/block_size);
					}

					unsigned cur_block = 0;
					std::vector<unsigned> reserved;
					while(cur_block != block_sets->num_blocks && additional_blocks != 0){
						if(!block_sets->blocks.at(cur_block)){
							block_sets->blocks.at(cur_block) = true;
							reserved.push_back(cur_block);
							--additional_blocks;
						}
						cur_block++;
					}
					if (additional_blocks){
						std::cerr << "NOT ENOUGH SPACE FOR FILE. BYTES NOT ADDED." << std::endl;
						for (unsigned b : reserved) block_sets->blocks.at(b)= false;
						to_mod->remaining = remaining;
						reserved.clear();
					} else {
						for(auto ind : reserved) to_mod->lf.push_back(ind);
						to_mod->size += bytes;
					}

				/************ remove **********/
				} else if (cmd == "remove"){
					//TODO: remove bytes from given file, make sure it's a file and not a directory



				} else {
					std::cerr << "Invalid command!" << std::endl;
					std::cout << valid_commands() << std::endl;
				}
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
