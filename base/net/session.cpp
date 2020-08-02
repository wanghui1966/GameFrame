#include "session.h"

#include "../debug.h"

int Session::GetId() const
{
	return m_session_id;
}

int Session::GetRecvMaxSize() const
{
	return m_recv_buf.GetCanWriteSize();
}

bool Session::CanRecv(int size) const
{
	return (size <= GetRecvMaxSize());
}

bool Session::OnRecv(const char *buf, int size)
{
	if (!CanRecv(size))
	{
		ELOG("Session::OnRecv fail for CanRecv:session_id=%d, recv_max_size=%d, size=%d", m_session_id, GetRecvMaxSize(), size);
		return false;
	}

	m_recv_buf.Write(buf, size);

	m_recv_buf.TryDecode();
	return true;
}

int Session::GetSendMaxSize() const
{
	return m_send_buf.GetCanReadSize();
}

bool Session::CanSend(int size) const
{
	return (size <= GetSendMaxSize());
}

bool Session::OnSend(char *buf, int size)
{
	if (!CanSend(size))
	{
		ELOG("Session::OnSend fail for CanSend:session_id=%d, send_max_size=%d, size=%d", m_session_id, GetSendMaxSize(), size);
		return false;
	}

	return m_send_buf.Read(buf, size);
}
