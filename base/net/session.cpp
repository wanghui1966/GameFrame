#include "session.h"
#include "net_manager.h"

#include "../debug.h"

int Session::GetId() const
{
	return m_session_id;
}

int Session::GetRecvMaxSize() const
{
	return m_recv_buf.GetCanWriteSize();
}

bool Session::OnRecv(const char *buf, int size)
{
	int can_write_size = m_recv_buf.GetCanWriteSize();
	if (size > can_write_size)
	{
		ELOG("Session::OnRecv fail for recv_buf size:session_id=%d, fd=%d, can_write_size=%d, size=%d", m_session_id, m_fd, can_write_size, size);
		return false;
	}

	m_recv_buf.Write(buf, size);

	m_recv_buf.TryDecode(m_session_id);
	return true;
}

int Session::GetSendMaxSize() const
{
	return m_send_buf.GetCanWriteSize();
}

bool Session::Send(Packet &packet)
{
	packet.PackageData();
	int size = packet.GetSize();

	int can_write_size = GetSendMaxSize();
	if (size > can_write_size)
	{
		ELOG("Session::Send fail for send_buf size:session_id=%d, fd=%d, opcode=%u, can_write_size=%d, size=%d", m_session_id, m_fd, packet.GetOpcode(), can_write_size, size);
		return false;
	}

	if (!m_send_buf.Write(packet.GetData(), size))
	{
		ELOG("Session::Send fail for Write:session_id=%d, fd=%d, opcode=%u, can_write_size=%d, size=%d", m_session_id, m_fd, packet.GetOpcode(), can_write_size, size);
		return false;
	}

	sNetManager.SendReady(m_session_id);

	DLOG("Session::Send:session_id=%d, fd=%d, opcode=%u, size=%d", m_session_id, m_fd, packet.GetOpcode(), size);
	return true;
}

bool Session::OnSend()
{
	return m_send_buf.OnSend(m_session_id, m_fd);
}
