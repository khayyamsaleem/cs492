#pragma once

#include <sstream>
#include <utility>

struct disk_node {
	unsigned block_start;
	unsigned block_end;
	bool used;
	disk_node * next;

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
	disk_node * head = nullptr;

	bool add(disk_node &n) {
		if (head == nullptr) {
			head = &n;
		} else {
			disk_node * cur = this->head;
			while (cur->next != nullptr)
				cur = cur->next;
			cur->next = &n;
		}
		return true;
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

	friend std::ostream& operator<<(std::ostream& os, const ldisk &l) {
		os << "ldisk { ";
		for (disk_node * cur = l.head; cur != nullptr; cur = cur->next)
			os << *cur << ", ";
		os << "\b\b }";
		return os;
	}
};
