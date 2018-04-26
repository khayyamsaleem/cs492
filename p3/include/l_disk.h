/*
 * l_disk.h
 *
 *  Created on: Apr 25, 2018
 *      Author: khayyamsaleem
 */
#include <vector>
#include <iostream>

#pragma once
struct l_disk {
	std::vector<bool> blocks;
	unsigned disk_size;
	unsigned block_size;
	unsigned num_blocks;

	l_disk(unsigned disk_size, unsigned block_size) {
		this->disk_size = disk_size;
		this->block_size = block_size;
		this->num_blocks = disk_size / block_size;
		this->blocks.reserve(num_blocks);
		this->blocks.resize(num_blocks);
		for(int i = 0; i < this->blocks.size(); i++){
			this->blocks.at(i) = false;
		}
	}

	std::vector<bool>::iterator find_first_free() {
		for (auto it = this->blocks.begin(); it != this->blocks.end(); ++it)
			if (!(*it))
				return it;
		return this->blocks.end();
	}

	friend std::ostream& operator<<(std::ostream& os, l_disk &disk) {
		int rel = 0;
		for (int i = 0; i < disk.num_blocks; i++) {
			std::string status = disk.blocks.at(i) ? "USED: " : "FREE: ";
			if (i == disk.num_blocks - 1) {
				if (rel == i)
					std::cout << status << rel << std::endl;
				else
					std::cout << status << rel << "-" << i << std::endl;
			} else if (disk.blocks.at(i) != disk.blocks.at(i + 1)) {
				if (rel == i)
					std::cout << status << rel << std::endl;
				else
					std::cout << status << rel << "-" << i << std::endl;
				rel = i + 1;
			}
		}
		os.flush();
		return os;
	}
};
