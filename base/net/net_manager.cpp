#include "net_manager.h"

#include "session.h"

#include <cstring>

#include "../debug.h"

int NetManager::m_session_count = 0;

NetManager::~NetManager()
{
	// while (close(m_epoll_fd) == -1 && errno == EINTR);
	DLOG("NetManager::~NetManager success:size=%zu", m_connect_session.size());
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
		ELOG("NetManager::Open fail for epoll_ctl_add:epoll_fd=%d, listen_fd=%d", m_epoll_fd, m_listen_fd);
		return false;
	}

	NLOG("NetManager::Open success:epoll_fd=%d, listen_fd=%d", m_epoll_fd, m_listen_fd);
	return true;
}

bool NetManager::Send(int session_id, Packet &packet)
{
	auto it_fd = m_session_fd.find(session_id);
	if (it_fd == m_session_fd.end())
	{
		return false;
	}

	auto it = m_connect_session.find(it_fd->second);
	if (it == m_connect_session.end())
	{
		return false;
	}

	return it->second->Send(packet);
}

void NetManager::SendReady(int session_id)
{
	auto it = m_session_fd.find(session_id);
	if (it == m_session_fd.end())
	{
		ELOG("NetManager::SendReady fail for session:session_id=%d", session_id);
		return;
	}

	m_ready_write_fd.insert(it->second);
	DLOG("NetManager::SendReady:session_id=%d, fd=%d, ready_write_fd_size=%zu", session_id, it->second, m_ready_write_fd.size());
}

void NetManager::Heartbeat()
{
	// 需要发送数据的套接字的监听事件加入EPOLLOUT
	if (!m_ready_write_fd.empty())
	{
		std::set<int> need_remove_fd;
		struct epoll_event ev;
		for (auto it = m_ready_write_fd.begin(); it != m_ready_write_fd.end(); ++it)
		{
			ev.events = EPOLLIN | EPOLLOUT;
			ev.data.fd = *it;
			if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, *it, &ev) == -1)
			{
				ELOG("NetManager::Heartbeat fail for epoll_ctl_mod begin:epoll_fd=%d, fd=%d", m_epoll_fd, *it);
				need_remove_fd.insert(*it);
				continue;
			}
			DLOG("NetManager::Heartbeat success fo epoll_ctl_mod begin:epoll_fd=%d, fd=%d", m_epoll_fd, *it);
		}
		for (auto it = need_remove_fd.begin(); it != need_remove_fd.end(); ++it)
		{
			m_ready_write_fd.erase(*it);
		}
	}

	struct epoll_event events[MAX_FD_NUM];
	int ready_fd = epoll_wait(m_epoll_fd, events, MAX_FD_NUM, -1);
	if (ready_fd <= 0)
	{
		DLOG("NetManager::Heartbeat:no ready_fd");
		return;
	}

	std::set<int> write_ok_fd;
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
			if (EventPollOut(events[i].events, events[i].data.fd))
			{
				m_ready_write_fd.erase(events[i].data.fd);
				write_ok_fd.insert(events[i].data.fd);
			}
		}
		else
		{
			EventPollClose(events[i].events, events[i].data.fd);
		}
	}

	// 数据发送完毕，套接字的监听事件去掉EPOLLOUT
	if (!write_ok_fd.empty())
	{
		struct epoll_event ev;
		for (auto it = write_ok_fd.begin(); it != write_ok_fd.end(); ++it)
		{
			ev.events = EPOLLIN;
			ev.data.fd = *it;
			if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, *it, &ev) == -1)
			{
				ELOG("NetManager::Heartbeat fail for epoll_ctl_mod end:epoll_fd=%d, fd=%d", m_epoll_fd, *it);
				continue;
			}
			DLOG("NetManager::Heartbeat success fo epoll_ctl_mod end:epoll_fd=%d, fd=%d", m_epoll_fd, *it);
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
	++m_session_count;
	m_connect_session.insert(std::make_pair(connect_fd, new Session(m_session_count, connect_fd)));
	m_session_fd.insert(std::make_pair(m_session_count, connect_fd));

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
	int recv_max_size = it->second->GetRecvMaxSize();
	if (recv_max_size <= 0)
	{
		ELOG("NetManager::EventPollIn fail for recv_max_size:events=%u, fd=%d", events, fd);
		return;
	}

	char buf[MAX_BUF_SIZE] = {0};
	memset(buf, 0, MAX_BUF_SIZE);

	ssize_t len = read(fd, buf, std::min(recv_max_size, MAX_BUF_SIZE));
	if (len > 0)
	{
		DLOG("NetManager::EventPollIn:events=%u, fd=%d, len=%zu", events, fd, len);
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

bool NetManager::EventPollOut(uint32_t events, int fd)
{
	auto it = m_connect_session.find(fd);
	if (it == m_connect_session.end() || !it->second)
	{
		ELOG("NetManager::EventPollOut fail for session:events=%u, fd=%d", events, fd);
		return false;
	}

	DLOG("NetManager::EventPollOut:events=%u, fd=%d", events, fd);
	return it->second->OnSend();
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
		m_session_fd.erase(it->second->GetId());
		delete it->second;
		m_connect_session.erase(it);
	}
	NLOG("NetManager::EventPollClose close socket:events=%u, fd=%d, err=%d", events, fd, err);
}
