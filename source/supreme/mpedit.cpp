#include "mpedit.h"
#include <string.h>
#include <iostream>
#include <algorithm>
#include "hamworld.h"
#include "sockets.h"

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
	statusBuf = "asleep...";
}

Sync Multiplayer::begin_sync() {
	if (host) {
		sockets::BufferedSocket peer = std::move(host.accept());
		if (peer) {
			peer.set_blocking(false);
			peers.push_back(std::move(peer));
		}
	}

	for (sockets::BufferedSocket& peer : peers) {
		peer.send_more();
		while (peer.recv_more()) {
			// TODO: this sure does involve a lot of copying
			std::istringstream buf(std::string((char*) peer.recv_buf.data(), peer.recv_buf.size()));
			size_t length_prefix = 0;
			if (!hamworld::read_varint(buf, &length_prefix))
				continue;
			size_t start = buf.tellg();
			if (peer.recv_buf.size() < start + length_prefix)
				continue;

			while (buf.tellg() < start + length_prefix) {
				size_t depth = 0;
				hamworld::read_varint(buf, &depth);

				Data *ptr = &data;
				size_t idx;
				for (size_t i = 0; i < depth; ++i) {
					hamworld::read_varint(buf, &idx);
					ptr = ptr->child(idx);
				}

				size_t value_len = 0;
				hamworld::read_varint(buf, &value_len);
				ptr->value.resize(value_len);
				buf.read(ptr->value.data(), value_len);
				ptr->needs_read = true;
			}

			//printf("consuming: %d\n", buf.tellg());
			peer.consume(start + length_prefix);
		}
	}

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

	std::string data = output.str();
	if (data.size()) {
		std::ostringstream length_prefix;
		hamworld::write_varint(length_prefix, data.size());
		std::string prefix = length_prefix.str();

		for (sockets::BufferedSocket& peer : peers) {
			//printf("sending packet: %d\n", (int)data.size());
			peer.send_all((uint8_t*) prefix.data(), prefix.size());
			peer.send_all((uint8_t*) data.data(), data.size());
			peer.send_more();
		}
	}
}

bool Multiplayer::active() {
	return host || peers.size();
}

const char* Multiplayer::status() {
	return statusBuf.c_str();
}

void Multiplayer::startHosting() {
	statusBuf = "Failed";

	sockets::Address res = sockets::lookup(nullptr, "10027");
	sockets::Socket sock(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (!sock) return;
	if (!sock.bind(res.get())) return;
	if (!sock.listen()) return;
	if (!sock.set_blocking(false)) return;
	host = std::move(sock);

	statusBuf = "Hosting server";
}

void Multiplayer::connectTo(const char* addr) {
	sockets::Address set = sockets::lookup("localhost", "10028");

	host.clear();
	statusBuf.clear();
	peers.clear();
	for (addrinfo *ptr = set.get(); ptr; ptr = ptr->ai_next) {
		sockets::BufferedSocket sock;
		if (!sock.create(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) continue;
		if (!sock.connect(ptr)) continue;
		if (!sock.set_blocking(false)) continue;

		peers.push_back(std::move(sock));
		break;
	}

	if (peers.empty()) {
		statusBuf = "Failed";
	} else {
		statusBuf = "Connected to server";
	}
}

}  // namespace mp
