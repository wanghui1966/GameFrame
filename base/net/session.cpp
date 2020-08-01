#include "session.h"

#include "../debug.h"
#include "../packet/packet.h"

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

	while (true)
	{
		uint32_t data_length = m_recv_buf.GetPacketDataLength();
		if (data_length == 0)
		{
			break;
		}
		int length = data_length + PACKET_HEADER_SIZE;

		if (length < m_recv_buf.GetCanReadSize())
		{
			break;
		}

		char data_length_buf[PACKET_DATA_LENGTH_SIZE] = {0};
		m_recv_buf.Read(data_length_buf, PACKET_DATA_LENGTH_SIZE);

		char opcode_buf[PACKET_OPCODE_SIZE] = {0};
		m_recv_buf.Read(opcode_buf, PACKET_OPCODE_SIZE);
		uint32_t opcode = *((uint32_t*)opcode_buf);
		NLOG("xxxxxxxx:%d, %d", data_length, opcode);

		char data_buf[SESSION_CIRCLE_BUFFER_CAPACITY];
		m_recv_buf.Read(data_buf, data_length);

		Data server_data;
		// 解包
		Packet server_packet(opcode, data_length);
		server_packet.UnpackageData(data_buf, data_length);
		server_data.PacketTo(server_packet);

		std::string server_data_str;
		server_data.GetStr(server_data_str);
		NLOG("Session::OnRecv:session_id=%d, opcode=%u, data_length=%u, data=[%s]", m_session_id, opcode, data_length, server_data_str.c_str());
	}

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
