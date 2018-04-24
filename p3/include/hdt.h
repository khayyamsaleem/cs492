#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <queue>
#include <assert.h>
#include <cstring>
#include <stack>

enum kind {DIR_T, FILE_T};

struct File {
    double size;
    std::string file_path;
    std::chrono::system_clock::time_point time_stamp;
    kind type;

    File(double size, std::string file_path, std::chrono::system_clock::time_point time_stamp, kind type){
        this->size = size;
        this->file_path = file_path;
        this->time_stamp = time_stamp;
        this->type = type;
    }

    double get_size(){
        return this->size;
    }

    std::string get_path(){
        return this->file_path;
    }

    std::chrono::system_clock::time_point get_time_stamp(){
        return this->time_stamp;
    }

    friend std::ostream& operator<<(std::ostream& os, File &f){
        std::time_t t =  std::chrono::system_clock::to_time_t(f.get_time_stamp());
        std::string time_string = std::ctime(&t);
        time_string = time_string.substr(0, time_string.length()-1);
        if (f.type==DIR_T)
            os << "DIR ";
        else
            os << "FILE";
        os << " [ size=" << f.get_size() <<
                    "; path=" << f.get_path() << " ]";
        return os;
    }

//    friend std::stack<File*> trace(File* dir){
//        assert(dir->type == DIR_T);
//        std::stack<File*> out;
//        for(File* cur = dir; cur; cur = cur->parent){
//            assert(cur->type == DIR_T);
//            out.push(cur);
//        }
//        return out;
//    }


};

struct hdt {
	File *data;
	std::vector<hdt*> children;
	hdt* parent;

	hdt(File* f){
		this->data = f;
		this->parent = nullptr;
	}

	void insert_node(File *f){
		hdt* parent = this->find(f->file_path.substr(0,f->file_path.find_last_of("/")));
		hdt* child = new hdt(f);
		parent->addChild(child);
	}

	/*friend */void addChild(/*hdt* parent, */hdt* child){
		this->children.push_back(child);
		child->parent = this;
	}

	/*friend */hdt* find(/*hdt* localroot, */std::string path){
//		if(localroot){
			hdt* cur = this;
			std::queue<hdt*> q;
			while(cur){
				if(cur->data->file_path == path)
					return cur;
				for(auto it = cur->children.begin(); it != cur->children.end(); ++it)
					q.push(*it);
				if(q.empty()) cur = nullptr;
				else {
					cur = q.front();
					q.pop();
				}
			}
			return cur;
//		} else {
//			return nullptr;
//		}
	}



    friend std::ostream& operator<<(std::ostream& os, hdt &t){
    	os << "TREE { " << std::endl;
		hdt* cur = &t;
		std::queue<hdt*> q;
		while(cur){
			os << "         " << *(cur->data);
			for(auto it = cur->children.begin(); it != cur->children.end(); ++it)
				q.push(*it);
			if(q.empty())
				cur = NULL;
			else {
				cur = q.front();
				q.pop();
			}
			os << std::endl;
		}
		os << "}";
    	return os;
    }
};



