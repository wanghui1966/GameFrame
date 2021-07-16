#pragma once

#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include <sys/epoll.h>

#include <string>
#include <map>
#include <set>

#include "../singleton.h"

// 服务器收到客户端的ack包完成三次连接，等待accept函数的队列长度，取此值和/proc/sys/net/core/somaxconn的最小值。
const int BACKLOG = 5;

const int MAX_BUF_SIZE = 1024;

// 从读缓冲区读的最小数据长度
const int MIN_RECV_SIZE = 10;

// 写写缓冲区的最小数据长度
const int MIN_SEND_SIZE = 1;

// 最大描述符数量
const int MAX_FD_NUM = 1024;

extern int errno;

class Packet;
class Session;
class NetManager : public Singleton<NetManager>
{
public:
	NetManager() {}
	virtual ~NetManager();

public:
	bool Init(const std::string &server_ip = "127.0.0.1", in_port_t server_port = 6666);
	bool Open();
	void Heartbeat();
	bool Send(int session_id, Packet &packet);
	void SendReady(int session_id);

protected:
	// 监听套接字
	void EventListen(uint32_t events);

	// 可读事件或错误
	void EventPollIn(uint32_t events, int fd);

	// 可写事件
	bool EventPollOut(uint32_t events, int fd);

	// 关闭连接事件
	void EventPollClose(uint32_t events, int fd);

protected:
	std::string				m_server_ip = "127.0.0.1";	// 服务器ip
	in_port_t				m_server_port = 6666;		// 服务器port

	int						m_listen_fd = 0;			// 监听套接字
	int						m_epoll_fd = 0;				// 读写套接字
	std::map<int, Session*>	m_connect_session;			// 所有的连接套接字，key=fd
	std::map<int, int>		m_session_fd;				// session_id和fd的映射关系，key=session，value=fd

	std::set<int>			m_ready_write_fd;			// 所有准备好投递写请求的套接字

	static int				m_session_count;			// 分配过的session的数量
};
#define sNetManager (*NetManager::Instance())

#endif
