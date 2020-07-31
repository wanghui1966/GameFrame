#include "net_manager.h"
#include "session.h"

#include <cstring>

#include "../debug.h"

int NetManager::m_session_count = 0;

NetManager::~NetManager()
{
	// while (close(m_epoll_fd) == -1 && errno == EINTR);
	NLOG("NetManager::~NetManager success:size=%zu", m_connect_session.size());
	while (true)
	{
		if (m_connect_session.empty())
		{
			break;
		}

		EventPollClose(EPOLLERR, m_connect_session.begin()->first);
	}

	close(m_listen_fd);
	close(m_epoll_fd);
}

bool NetManager::Init(const std::string &server_ip/* = "127.0.0.1"*/, in_port_t server_port/* = 6666*/)
{
	if (server_ip.empty() || server_port <= 0)
	{
		ELOG("NetManager::Init fail:server_ip=%s, server_port=%u", server_ip.c_str(), server_port);
		return false;
	}

	m_server_ip = server_ip;
	m_server_port = server_port;
	NLOG("NetManager::Init success:server_ip=%s, server_port=%u", m_server_ip.c_str(), m_server_port);
	return true;
}

bool NetManager::Open()
{
	// 创建epoll句柄
	m_epoll_fd = epoll_create(MAX_FD_NUM);
	if (m_epoll_fd < 0)
	{
		ELOG("NetManager::Open fail for epoll_create");
		return false;
	}

	// 创建套接字
	m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listen_fd < 0)
	{
		ELOG("NetManager::Open fail for socket");
		return false;
	}

	// 服务器套接字
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(m_server_port);
	server_addr.sin_addr.s_addr = inet_addr(m_server_ip.c_str());

	// 绑定套接字
	if (bind(m_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		ELOG("NetManager::Open fail for bind");
		close(m_listen_fd);
		return false;
	}

	// 监听
	if (listen(m_listen_fd, BACKLOG) < 0)
	{
		ELOG("NetManager::Open fail for listen");
		close(m_listen_fd);
		return false;
	}

	struct epoll_event ev;
	ev.data.fd = m_listen_fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &ev) == -1)
	{
		ELOG("NetManager::Open fail for epoll_ctl:epoll_fd=%d, listen_fd=%d", m_epoll_fd, m_listen_fd);
		return false;
	}

	NLOG("NetManager::Open success:epoll_fd=%d, listen_fd=%d", m_epoll_fd, m_listen_fd);
	return true;
}

void NetManager::Heartbeat()
{
	struct epoll_event events[MAX_FD_NUM];
	int ready_fd = epoll_wait(m_epoll_fd, events, MAX_FD_NUM, -1);
	if (ready_fd <= 0)
	{
		NLOG("NetManager::Heartbeat:no ready_fd");
		return;
	}

	for (int i = 0; i < ready_fd; ++i)
	{
		// 监听套接字消息
		if ((events[i].data.fd == m_listen_fd) && (events[i].events & (EPOLLIN | EPOLLERR | EPOLLHUP)))
		{
			EventListen(events[i].events);
		}
		else if (events[i].events & (EPOLLIN | EPOLLERR | EPOLLHUP))
		{
			EventPollIn(events[i].events, events[i].data.fd);
		}
		else if (events[i].events & (EPOLLOUT))
		{
			EventPollOut(events[i].events, events[i].data.fd);
		}
		else
		{
			EventPollClose(events[i].events, events[i].data.fd);
		}
	}
}

void NetManager::EventListen(uint32_t events)
{
	struct sockaddr_in listen_addr;
	socklen_t listen_addr_len = sizeof(listen_addr);
	int connect_fd = accept(m_listen_fd, (struct sockaddr*)&listen_addr, &listen_addr_len);
	if (connect_fd < 0)
	{
		ELOG("NetManager::EventListen new connect fail for accept:events=%u", events);
		return;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = connect_fd;
	if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, connect_fd, &ev) == -1)
	{
		ELOG("NetManager::EventListen new connect fail for epoll_ctl:events=%u, connect_fd=%d", events, connect_fd);
		close(connect_fd);
		return;
	}
	m_connect_session.insert(std::make_pair(connect_fd, new Session(++m_session_count)));

	NLOG("NetManager::EventListen new connect success:events=%u, connect_fd=%d, session_id=%d", events, connect_fd, m_session_count);
}

void NetManager::EventPollIn(uint32_t events, int fd)
{
	auto it = m_connect_session.find(fd);
	if (it == m_connect_session.end() || !it->second)
	{
		ELOG("NetManager::EventPollIn fail for session:events=%u, fd=%d", events, fd);
		return;
	}
	if (!it->second->CanRecv(MIN_RECV_SIZE))
	{
		ELOG("NetManager::EventPollIn fail for CanRecv:events=%u, fd=%d", events, fd);
		return;
	}

	char buf[MAX_BUF_SIZE] = {0};
	memset(buf, 0, MAX_BUF_SIZE);

	ssize_t len = read(fd, buf, std::min(it->second->GetRecvMaxSize(), MAX_BUF_SIZE));
	if (len > 0)
	{
		NLOG("NetManager::EventPollIn:events=%u, fd=%d, buf=%s", events, fd, buf);
		it->second->OnRecv(buf, len);
	}
	else if ((len == -1 && errno == EINTR) || errno == EAGAIN)
	{
		// 慢系统调用导致信号中断没有读到数据 或 非阻塞没有数据则返回EAGAIN
		ELOG("NetManager::EventPollIn fail for recv:events=%u, fd=%d, errno=%d", events, fd, errno);
	}
	else
	{
		NLOG("NetManager::EventPollIn close socket:events=%u, fd=%d", events, fd);
		EventPollClose(events, fd);
	}
}

void NetManager::EventPollOut(uint32_t events, int fd)
{
	auto it = m_connect_session.find(fd);
	if (it == m_connect_session.end() || !it->second)
	{
		ELOG("NetManager::EventPollOut fail for session:events=%u, fd=%d", events, fd);
		return;
	}
	if (it->second->CanSend(MIN_SEND_SIZE))
	{
		ELOG("NetManager::EventPollOut fail for CanSend:events=%u, fd=%d", events, fd);
		return;
	}

	char buf[MAX_BUF_SIZE] = {0};
	memset(buf, 0, MAX_BUF_SIZE);
	int size = std::min(it->second->GetSendMaxSize(), MAX_BUF_SIZE);
	if (it->second->OnSend(buf, size))
	{
		write(fd, buf, size);
	}
}

void NetManager::EventPollClose(uint32_t events, int fd)
{
	struct epoll_event ev;
	int err = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &ev);
	if (err != -1)
	{
		close(fd);
	}
	auto it = m_connect_session.find(fd);
	if (it != m_connect_session.end())
	{
		delete it->second;
		m_connect_session.erase(it);
	}
	NLOG("NetManager::EventPollClose close socket:events=%u, fd=%d, err=%d", events, fd, err);
}
