#pragma once

#ifndef SESSION_H
#define SESSION_H

#include "../circle_buffer/circle_buffer.h"
#include "../packet/packet.h"

// 缓冲区1K
//const int SESSION_CIRCLE_BUFFER_CAPACITY = 1048576;
const int SESSION_CIRCLE_BUFFER_CAPACITY = 256;

class Session
{
public:
	explicit Session(int session_id, int fd) : m_session_id(session_id), m_fd(fd), m_recv_buf(SESSION_CIRCLE_BUFFER_CAPACITY), m_send_buf(SESSION_CIRCLE_BUFFER_CAPACITY) {}
	virtual ~Session() {}

public:
	int GetId() const;

public:
	int GetRecvMaxSize() const;
	// 套接字收到数据后调用此函数，调用CircleBuffer的Write函数(将数据放入m_recv_buf)，调用CircleBuffer的TryDecode函数(尝试解码)
	bool OnRecv(const char *buf, int size);

	int GetSendMaxSize() const;
	// 序列化数据，将数据放入m_send_buf，调用NetManager的SendReady函数(套接字设置EPOLLOUT事件)
	bool Send(Packet &packet);
	// 调用CircleBuffer的OnSend函数(套接字write函数发送m_send_buf里的数据)
	bool OnSend();

protected:
	int							m_session_id = 0;
	int							m_fd = 0;

	CircleBuffer				m_recv_buf;				// 套接字的接受缓冲区
	CircleBuffer				m_send_buf;				// 套接字的发送缓冲区
};

#endif
