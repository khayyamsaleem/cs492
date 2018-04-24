#pragma once

#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>

#include "file.h"

struct hdt {
	File *data; /* file in node */
	std::vector<hdt*> children; /* all children of n-ary node */
	hdt* parent; /* reference to parent */

	/* constructs a hierarchical directory tree node given a File */
	hdt(File* f) {
		this->data = f;
		this->parent = nullptr;
	}

	/* inserts a node to the hierarchical directory tree */
	void insert_node(File *f) {
		hdt* parent = this->find(
				f->file_path.substr(0, f->file_path.find_last_of("/")));
		hdt* child = new hdt(f);
		parent->addChild(child);
	}

	/* just adds a single child to a node's children vector */
	void addChild(hdt* child) {
		this->children.push_back(child);
		child->parent = this;
	}

	/* returns reference to node given path (search with BFS) */
	hdt* find(std::string path) {
		hdt* cur = this;
		std::queue<hdt*> q;
		while (cur) {
			if (cur->data->file_path == path)
				return cur;
			for (auto it = cur->children.begin(); it != cur->children.end();
					++it)
				q.push(*it);
			if (q.empty())
				cur = nullptr;
			else {
				cur = q.front();
				q.pop();
			}
		}
		return cur;
	}

	/* performs a breadth-first traversal of a hierarchical directory tree */
	friend std::ostream& operator<<(std::ostream& os, hdt &t) {
		os << "TREE {" << std::endl;
		hdt* cur = &t;
		std::queue<hdt*> q;
		while (cur) {
			os << "\t" << *(cur->data);
			for (auto it = cur->children.begin(); it != cur->children.end();
					++it)
				q.push(*it);
			if (q.empty())
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

