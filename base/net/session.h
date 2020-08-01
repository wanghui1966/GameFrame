#pragma once

#ifndef SESSION_H
#define SESSION_H

#include "../circle_buffer/circle_buffer.h"
#include "../packet/packet.h"

#include "../debug.h"

// 缓冲区1K
//const int SESSION_CIRCLE_BUFFER_CAPACITY = 1048576;
const int SESSION_CIRCLE_BUFFER_CAPACITY = 256;

class Session
{
public:
	explicit Session(int session_id) : m_session_id(session_id), m_recv_buf(SESSION_CIRCLE_BUFFER_CAPACITY), m_send_buf(SESSION_CIRCLE_BUFFER_CAPACITY) {}
	virtual ~Session() {}

public:
	int GetId() { return m_session_id; }

public:
	int GetRecvMaxSize() const
	{
		return m_recv_buf.GetCanWriteSize();
	}
	bool CanRecv(int size) const
	{
		return (size <= GetRecvMaxSize());
	}
	bool OnRecv(const char *buf, int size)
	{
		if (!CanRecv(size))
		{
			ELOG("Session::OnRecv fail for CanRecv:session_id=%d, recv_max_size=%d, size=%d", m_session_id, GetRecvMaxSize(), size);
			return false;
		}

		m_recv_buf.Write(buf, size);

		while (true)
		{
			int data_length = m_recv_buf.GetPacketDataLength();
			if (data_length == 0)
			{
				break;
			}
			int length = data_length + PACKET_HEADER_SIZE;

			if (length < m_recv_buf.GetCanReadSize())
			{
				break;
			}

			char temp_buf[SESSION_CIRCLE_BUFFER_CAPACITY];
			m_recv_buf.Read(temp_buf, length);
			NLOG("Session::OnRecv:session_id=%d, buf=%s", m_session_id, temp_buf);
		}

		return true;
	}

	int GetSendMaxSize() const
	{
		return m_send_buf.GetCanReadSize();
	}
	bool CanSend(int size) const
	{
		return (size <= GetSendMaxSize());
	}
	bool OnSend(char *buf, int size)
	{
		if (!CanSend(size))
		{
			ELOG("Session::OnSend fail for CanSend:session_id=%d, send_max_size=%d, size=%d", m_session_id, GetSendMaxSize(), size);
			return false;
		}

		return m_send_buf.Read(buf, size);
	}

protected:
	int							m_session_id = 0;

	CircleBuffer				m_recv_buf;
	CircleBuffer				m_send_buf;
};

#endif
