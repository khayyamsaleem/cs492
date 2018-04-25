#pragma once
#include <ldisk.h>
#include <sstream>
#include <math.h>

struct file_block {
	double block_addr;
	file_block *next;
	
	file_block(double block_addr){
		this->block_addr = block_addr;
		this->next = nullptr;
	}
	
	file_block(double block_addr, file_block *next){
		this->block_addr = block_addr;
		this->next = next;
	}
	
	friend std::ostream& operator<<(std::ostream& os, file_block &fb){
		os << "FB [ " << fb.block_addr << " ]";
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

	bool add(ldisk *ld, double bytes){
		if(this->head == nullptr){
			//compute number of blocks to add to lfile
			unsigned num_blocks = ceil(this->block_size / bytes);
			for(unsigned i = 0; i < num_blocks; ++i){
				this->add(new file_block(ld->head->find_first_free()->block_start*this->block_size));
				ld->split(num_blocks);
			}
		}
		return true;
	}


	friend std::ostream& operator<<(std::ostream& os, lfile &l){
		for(file_block *cur = l.head; cur; cur = cur->next)
			os << cur;
		return os;
	}
};
