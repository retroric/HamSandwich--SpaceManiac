#ifndef SOCKETS_H
#define SOCKETS_H

#include <memory>
#include <vector>

#ifdef _WIN32
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#   include <winsock2.h>
#   include <ws2tcpip.h>
#else // _WIN32
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netdb.h>
#endif // _WIN32

namespace sockets {

struct _addrinfo_deleter {
	void operator()(addrinfo* info) { freeaddrinfo(info); }
};
typedef std::unique_ptr<addrinfo, _addrinfo_deleter> Address;
Address lookup(const char* host, const char* port);

class Socket;

class SocketRef {
	friend class Socket;
	int fd;
	SocketRef(int fd) : fd(fd) {}
public:
	SocketRef(const SocketRef&) = default;
	SocketRef& operator=(const SocketRef&) = default;
	SocketRef(SocketRef&&) noexcept = default;
	SocketRef& operator=(SocketRef&&) noexcept = default;

	inline operator bool() { return fd >= 0; };

	bool set_blocking(bool blocking);
	bool bind(const addrinfo *ai);
	bool listen();
	Socket accept();

	bool connect(const addrinfo *ai);
	int send(const void *msg, int len);
	int recv(void *buf, int len);
};

class Socket : public SocketRef {
	friend class SocketRef;
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
public:
	Socket() noexcept;
	Socket(int domain, int type, int protocol);
	Socket(Socket&&) noexcept;
	Socket& operator=(Socket&&) noexcept;
	inline ~Socket() { clear(); }

	void clear();
	bool create(int domain, int type, int protocol);
};

struct BufferedSocket : public Socket {
	std::vector<uint8_t> send_buf;
	std::vector<uint8_t> recv_buf;

	BufferedSocket() noexcept;
	BufferedSocket(Socket&&) noexcept;
	BufferedSocket& operator=(Socket&&) noexcept;
	BufferedSocket(BufferedSocket&&) noexcept = default;
	BufferedSocket& operator=(BufferedSocket&&) noexcept = default;

	void send_all(const uint8_t *msg, int len);
	void send_more();

	void consume(int len);
	bool recv_more();
};

}  // sockets

#endif  // SOCKETS_H
