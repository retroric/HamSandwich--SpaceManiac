#include "mpedit.h"
#include <string.h>
#include <iostream>
#include <algorithm>
#include "hamworld.h"

namespace mp {

Data* Data::child(size_t i) {
	if (children.size() <= i) {
		children.resize(i + 1);
	}
	return &children[i];
}

Data* Data::next_child() {
	size_t i = retain_up_to++;
	if (children.size() < retain_up_to) {
		children.resize(retain_up_to);
	}
	Data *child = &children[i];
	child->retain_up_to = 0;
	return child;
}

void Data::print() {
	std::cout << "{";
	for (size_t i = 0; i < retain_up_to; ++i) {
		children[i].print();
		std::cout << ",";
	}
	if (value.size()) {
		if (std::find(value.begin(), value.end(), '\0') != value.end()) {
			std::cout << &value[0];
		} else {
			std::cout << "binary";
		}
	}
	std::cout << "}";
}

void Sync::_field(char *buf, size_t len) {
	Data *ptr = data->next_child();

	if (ptr->needs_read) {
		size_t minlen = std::min(len, ptr->value.size());
		memcpy(buf, &ptr->value[0], minlen);
		ptr->needs_read = false;
	} else {
		if (len != ptr->value.size() || memcmp(buf, &ptr->value[0], len)) {
			ptr->value.assign(buf, buf + len);
			mp->queue(this, ptr);
		}
	}
}

Sync Sync::object() {
	return Sync(mp, this, data->next_child());
}

Multiplayer::Multiplayer()
{
}

Sync Multiplayer::begin_sync() {
	data.retain_up_to = 0;
	output.str("");
	return Sync(this, nullptr, &data);
}

void Multiplayer::queue(Sync *sync, Data *data) {
	if (initialized) {
		std::vector<size_t> position;
		while (sync) {
			position.push_back(sync->data->retain_up_to - 1);
			sync = sync->parent;
		}

		hamworld::write_varint(output, position.size());
		for (size_t i = position.size(); i > 0; --i) {
			hamworld::write_varint(output, position[i - 1]);
		}
		hamworld::write_varint(output, data->value.size());
		output.write(&data->value[0], data->value.size());
	}
}

void Multiplayer::finish_sync() {
	initialized = true;
	//data.print();
	//std::cout << std::endl;

	std::string stuff = output.str();
	if (!stuff.empty()) {
		std::ofstream meme("/home/tad/work/HamSandwich/meme.dat");
		meme << stuff;
	}
}

bool Multiplayer::active() {
	return true;
}

const char* Multiplayer::status() {
	return "";
}

}  // namespace mp
