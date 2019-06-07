#include "sockets.h"
#include <string.h>
#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif

namespace sockets {

#ifdef _WIN32

static bool called_wsa_startup = false;

static void cleanup() {
	WSACleanup();
}

void init() {
	if (!called_wsa_startup) {
		WSAData data;
		if (WSAStartup(MAKEWORD(2, 0), &data) != 0) {
			return;
		}
		atexit(cleanup);
		called_wsa_startup = true;
	}
}

int get_error() {
	return WSAGetLastError();
}

#else  // _WIN32

inline void init() {}

int get_error() {
	return errno;
}

#endif

Address lookup(const char* host, const char* port) {
	init();

	addrinfo hints;
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	if (!host) {    // if server...
		hints.ai_flags = AI_PASSIVE; // fill in my IP for me
	}

	int status;
	addrinfo *servinfo;  // will point to the results
	if ((status = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		servinfo = nullptr;
	}
	return Address(servinfo);
}

Socket::Socket() noexcept
	: SocketRef(-1)
{
}

Socket::Socket(Socket&& other) noexcept
	: SocketRef(other.fd)
{
	other.fd = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
	std::swap(fd, other.fd);
	return *this;
}

void Socket::clear() {
	if (fd >= 0) {
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		fd = -1;
	}
}

Socket::Socket(int domain, int type, int protocol)
	: SocketRef((init(), socket(domain, type, protocol)))
{
}

bool Socket::create(int domain, int type, int protocol) {
	clear();
	init();
	fd = socket(domain, type, protocol);
	return *this;
}

bool SocketRef::set_blocking(bool blocking) {
	if (fd < 0) return false;

#ifdef _WIN32
	unsigned long mode = blocking ? 0 : 1;
	return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) return false;

	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

bool SocketRef::bind(const addrinfo *info) {
	return ::bind(fd, info->ai_addr, info->ai_addrlen) >= 0;
}

bool SocketRef::listen() {
	return ::listen(fd, 10) >= 0;
}

Socket SocketRef::accept() {
	sockaddr_storage their_addr;
	socklen_t size = sizeof(their_addr);
	Socket result;
	result.fd = ::accept(fd, (sockaddr*) &their_addr, &size);
	return result;
}

bool SocketRef::connect(const addrinfo *info) {
	return ::connect(fd, info->ai_addr, info->ai_addrlen) >= 0;
}

int SocketRef::send(const void *msg, int len) {
	return ::send(fd, (char*) msg, len, 0);
}

int SocketRef::recv(void *buf, int len) {
	return ::recv(fd, (char*) buf, len, 0);
}

BufferedSocket::BufferedSocket() noexcept {
}

BufferedSocket::BufferedSocket(Socket&& other) noexcept
	: Socket(std::move(other))
{
}

void BufferedSocket::send_all(const uint8_t *msg, int len) {
	send_buf.insert(send_buf.end(), msg, msg + len);
}

void BufferedSocket::send_more() {
	size_t total_sent = 0;
	while (total_sent < send_buf.size()) {
		int sent = send(send_buf.data() + total_sent, send_buf.size() - total_sent);
		if (sent <= 0) break;
		total_sent += sent;
	}
	if (total_sent > 0) {
		send_buf.erase(send_buf.begin(), send_buf.begin() + total_sent);
	}
}

bool BufferedSocket::recv_more() {
	int initial_size = recv_buf.size();
	int added_size = 8192;
	recv_buf.resize(initial_size + added_size);
	int read = recv(&recv_buf[initial_size], added_size);
	if (read <= 0) {
		recv_buf.resize(initial_size);
		if (read == 0) {
			closed = true;
		}
		return false;
	}
	recv_buf.resize(initial_size + read);
	return true;
}

void BufferedSocket::consume(int len) {
	recv_buf.erase(recv_buf.begin(), recv_buf.begin() + len);
}

}  // sockets
