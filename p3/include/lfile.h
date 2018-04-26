#pragma once
#include <sstream>
#include <math.h>

struct file_block {
	double block_addr;
	file_block *next;
	
	file_block(unsigned block_start){
		this->block_addr = block_start;
		this->next = nullptr;
	}
	
	friend std::ostream& operator<<(std::ostream& os, file_block &fb){
		os << "FB [ block_addr=" << fb.block_addr << " ]";
		return os;
	}
};

struct lfile {
	file_block *head;
	double block_size;
	
	lfile(double block_size){
		this->head = nullptr;
		this->block_size = block_size;
	}
	
	bool add(file_block *fb){
		if(this->head == nullptr){
			this->head = fb;
		} else {
			file_block *cur;
			for(cur = this->head; cur->next; cur = cur->next);
			cur->next = fb;
		}
		return true;
	}

	friend std::ostream& operator<<(std::ostream& os, lfile &l){
		os << "LFILE { ";
		for(file_block *cur = l.head; cur; cur = cur->next)
			os << *cur << ", ";
		os << "\b\b }";
		return os;
	}
};
