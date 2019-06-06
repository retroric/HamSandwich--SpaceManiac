#ifndef MPEDIT_H
#define MPEDIT_H

#include <vector>
#include <sstream>
#include "jamultypes.h"
#include "sockets.h"

namespace mp {

enum class Mode {
	Normal,
	Host,
	Join,
};

class Multiplayer;

struct Data {
	bool needs_read = false;
	size_t retain_up_to = 0;

	std::vector<Data> children;
	std::vector<char> value;

	Data *child(size_t i);
	Data *next_child();
	void print();
};

class Sync {
	Multiplayer *mp;
	Sync *parent;
	Data *data;

	Sync(Multiplayer *mp, Sync *parent, Data *data) : mp(mp), parent(parent), data(data) {}
	void _field(char *buf, size_t len);

	friend class Multiplayer;

public:
	Sync object();

	//template<int N> void field(char (&value)[N]) { return _field(value, N); }
	template<typename T>
	void field(T &value) { return _field((char*) &value, sizeof(T)); }
};

class Multiplayer {
	Data data;
	bool initialized = false;
	std::ostringstream output;
	std::string statusBuf;

	sockets::Socket host;
	std::vector<sockets::BufferedSocket> peers;

	void queue(Sync *sync, Data *data);
	friend class Sync;

public:
	Multiplayer();

	Sync begin_sync();
	void finish_sync();

	bool active();
	const char* status();

	void startHosting();
	void connectTo(const char* addr);
};

}  // namespace mp

#endif  // MPEDIT_H
