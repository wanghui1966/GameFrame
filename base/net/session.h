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
	bool OnRecv(const char *buf, int size);

	int GetSendMaxSize() const;
	bool Send(Packet &packet);
	bool OnSend();

protected:
	int							m_session_id = 0;
	int							m_fd = 0;

	CircleBuffer				m_recv_buf;
	CircleBuffer				m_send_buf;
};

#endif
