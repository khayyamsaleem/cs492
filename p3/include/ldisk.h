#pragma once

#include <sstream>
#include <utility>
#include <math.h>

struct disk_node {
	unsigned block_start;
	unsigned block_end;
	bool used;
	disk_node * next;

	disk_node* find_first_free(){
		for(disk_node* cur = this; cur; cur = cur->next){
			if(!cur->is_used())
				return cur;
		}
		return nullptr;
	}

	disk_node(unsigned s, unsigned e, bool used, disk_node * next) {
		this->block_start = s;
		this->block_end = e;
		this->used = used;
		this->next = next;
	}

	disk_node(unsigned s, unsigned e, bool used) {
		this->block_start = s;
		this->block_end = e;
		this->used = used;
		this->next = nullptr;
	}

	std::pair<unsigned, unsigned> get_block_ids() {
		return std::make_pair(block_start, block_end);
	}

	bool is_used() {
		return this->used;
	}

	friend std::ostream& operator<<(std::ostream& os, disk_node &d) {
		os << "d_node [ range=(" << d.get_block_ids().first << ","
				<< d.get_block_ids().second << "); ";
		if (d.used)
			os << "used";
		else
			os << "free";
		os << " ]";
		return os;
	}
};

struct ldisk {
	disk_node * head;

	ldisk(){
		this->head = nullptr;
	}

	bool add(disk_node *n) {
		if (head == nullptr) {
			head = n;
		} else {
			disk_node * cur = this->head;
			while (cur->next != nullptr)
				cur = cur->next;
			cur->next = n;
		}
		this->merge();
		return true;
	}

	bool insert_at(disk_node* pos, disk_node* ins){
		if (pos == this->head){
			this->head = new disk_node(ins->block_start, ins->block_end, ins->used, this->head);
			return true;
		}
		disk_node* prev = nullptr;
		for(disk_node* cur = this->head; cur; cur = cur->next){
			if(cur == pos){
				prev->next = new disk_node(ins->block_start, ins->block_end, ins->used, cur);
				return true;
			}
			prev = cur;
		}
		return false;
	}

	void merge() {
		disk_node * cur = this->head;
		while (cur->next != nullptr) {
			disk_node * n = cur->next;
			if (cur->used == n->used) {
				cur->block_end = n->block_end;
				cur->next = n->next;
			} else {
				cur = cur->next;
			}
		}
	}


	void split(unsigned cut_point){
		disk_node* cur = this->head;
		disk_node* prev = nullptr;
		while(cur){
			if (cur->block_start <= cut_point && cur->block_end >= cut_point){
				if (cur == this->head){
					this->head = new disk_node(0,cut_point,this->head->used, this->head);
					this->head->next->block_start=cut_point+1;
//					this->merge();
					return;
				} else {
					prev->next = new disk_node(cur->block_start, cut_point, cur->used, cur);
					cur->block_start=cut_point+1;
//					this->merge();
					return;
				}
			}
			prev = cur;
			cur = cur->next;
		}
	}

	void use_block_ids(unsigned start, unsigned end){
		this->split(start-1);
		this->split(end);
		disk_node *cur = this->head;
		while(cur->block_start != start)
			cur = cur->next;
		cur->used = true;
	}

	void free_block_ids(unsigned start, unsigned end){
		this->split(start-1);
		this->split(end);
		disk_node *cur = this->head;
		while(cur->block_start != start)
			cur = cur->next;
		cur->used = false;
	}

	friend std::ostream& operator<<(std::ostream& os, const ldisk &l) {
		disk_node *cur;
		os << "ldisk { ";
		for (cur = l.head; cur -> next ; cur = cur->next)
			os << *cur << ", ";
		os << *cur <<" }";
		return os;
	}
};
