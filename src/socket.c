#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

static void _socket_react(cpu *_cpu, int evfd); // forward declaration
												
static void _socket_incio(cpu *_cpu) {
	if (_cpu->bus.cells[COUNT_PORT] < 65535) {
		_cpu->bus.cells[COUNT_PORT]++;
	}
	else
		printf("[WARN] Too many connections to socket (more than 65535)\n");
}

static void _socket_decio(cpu *_cpu) {
	if (_cpu->bus.cells[COUNT_PORT] > 0) {
		_cpu->bus.cells[COUNT_PORT]--;
	}
	else
		printf("[ERR] Tried to remove too many connections\n");
}

void sigpipe_handler(int sig) {
	(void)sig;
	// skip SIGPIPE (may appear when terminal closes connection)
	return;
}

int socket_init(cpu *_cpu, char *path) {
	if (strlen(path) == 0) {
		printf("[PANIC] Length of socket path is 0\n");
		return 1;
	}

	unlink(path);

	asprintf(&_cpu->socket.sock_path, "%s", path);

	// SOCK_NONBLOCK is essential.
	_cpu->socket.server_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);	
	_cpu->socket.server_addr.sun_family = AF_UNIX;
	strcpy(_cpu->socket.server_addr.sun_path, _cpu->socket.sock_path);

	int server = 
		bind(_cpu->socket.server_socket, (struct sockaddr *)&_cpu->socket.server_addr, sizeof(_cpu->socket.server_addr));

	if (server == -1) {
		printf("[PANIC] Failed to bind.\n");
		return 1;
	} else {
		printf("[Server]: Started listening to unix socket.\n");
		printf("[Server]: Unix socket is at %s\n", _cpu->socket.sock_path);
	}

    signal(SIGPIPE, sigpipe_handler); // don't react to SIGPIPE
	_cpu->socket.is_connected = true;
	listen(_cpu->socket.server_socket, CONNECTIONS);
	_cpu->socket.clen = sizeof(_cpu->socket.client_addr);

	_cpu->socket.epoll.plfd = epoll_create(CONNECTIONS);

	_cpu->socket.epoll.ev.data.fd = _cpu->socket.server_socket;
	_cpu->socket.epoll.ev.events = EPOLLIN; 

	epoll_ctl(_cpu->socket.epoll.plfd, EPOLL_CTL_ADD, _cpu->socket.server_socket, &_cpu->socket.epoll.ev);

	return 0;
}

void socket_checkdata(cpu *_cpu) {
	// 0 -> non blocking
	size_t evs = epoll_wait(_cpu->socket.epoll.plfd, _cpu->socket.epoll.evs, BUF_EVS, 0);
	switch (evs) {
		case -1:
			printf("[ERROR] epoll_wait failed. It shouldn't happen.\n");
			break;
		case 0:
			// No data received;
			break;
		default:
			// there is data!
			if (_cpu->socket.epoll.evs[0].events & EPOLLHUP) {
				printf("[INFO] Terminal disconnected.\n");
				_socket_decio(_cpu);
			}
			for (size_t ev = 0; ev < evs; ++ev) {
				if (_cpu->socket.epoll.evs[ev].data.fd == _cpu->socket.server_socket) {
				printf("[INFO] Terminal connected.\n");
					_socket_incio(_cpu);
					_cpu->socket.client_socket = accept(_cpu->socket.server_socket, 
							(struct sockaddr *)&_cpu->socket.client_addr, &_cpu->socket.clen);
					if (_cpu->socket.client_socket < 0)
						continue;
					int flags = fcntl(_cpu->socket.client_socket, F_GETFL, 0);
					fcntl(_cpu->socket.client_socket, F_SETFL, flags | O_NONBLOCK);
					_cpu->socket.epoll.ev.data.fd = _cpu->socket.client_socket;
					_cpu->socket.epoll.ev.events = EPOLLIN;
					epoll_ctl(_cpu->socket.epoll.plfd, EPOLL_CTL_ADD, 
							_cpu->socket.client_socket, &_cpu->socket.epoll.ev);
				} else {
					_socket_react(_cpu, _cpu->socket.epoll.evs[ev].data.fd);			
				}
			}
			break;
	}
}

static void _socket_react(cpu *_cpu, int evfd) {
	char recv_byte = 0x00;
	if (recv(evfd, &recv_byte, sizeof(char), 0) <= 0) {
		// no data on this event, closing.
		close(evfd);
	} else {
		_cpu->bus.cells[INPUT_PORT] = (u16)recv_byte;
	}
}

void socket_write(cpu *_cpu, u16 addr, u16 val) {
	switch (addr) {
		case OUTPUT_PORT:
			// sending only one byte (this is 8 bit connection)
			send(_cpu->socket.client_socket, &val, sizeof(char), 0);
			break;
		case INPUT_PORT:
			// writing to input is just reset 
			break;
		default:
			printf("[WARN] 0x%04x port is not implemented for writing\n", addr);
			break;
	}
}

