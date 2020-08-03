#include "circle_buffer.h"

#include "../packet/packet.h"
#include <cstring>

#include "../debug.h"

CircleBuffer::CircleBuffer(int buf_capacity) : m_buf_capacity(buf_capacity)
{
	m_buf = new char[m_buf_capacity];
	memset(m_buf, 0, m_buf_capacity);
	NLOG("CircleBuffer create success:buf_capacity=%d", m_buf_capacity);
}

CircleBuffer::~CircleBuffer()
{
	NLOG("CircleBuffer destory success");
	if (m_buf)
	{
		delete m_buf;
		m_buf = nullptr;
	}
	m_buf_capacity = 0;
}

CircleBuffer::CircleBuffer(const CircleBuffer &cb)
{
	this->m_buf_capacity = cb.m_buf_capacity;
	m_buf = new char[m_buf_capacity];
	memset(m_buf, 0, m_buf_capacity);
	this->m_read_pos = cb.m_read_pos;
	this->m_write_pos = cb.m_write_pos;
	NLOG("CircleBuffer copy success:buf_capacity=%d", m_buf_capacity);
}

CircleBuffer& CircleBuffer::operator=(const CircleBuffer &cb)
{
	if (this != &cb)
	{
		if (m_buf)
		{
			delete m_buf;
		}

		this->m_buf_capacity = cb.m_buf_capacity;
		m_buf = new char[m_buf_capacity];
		memset(m_buf, 0, m_buf_capacity);
		this->m_read_pos = cb.m_read_pos;
		this->m_write_pos = cb.m_write_pos;
		NLOG("CircleBuffer = success:buf_capacity=%d", m_buf_capacity);
	}
	return *this;
}

int CircleBuffer::GetCanWriteSize() const
{
	if (m_read_pos <= m_write_pos)
	{
		int has_write_size = m_write_pos - m_read_pos;
		return (m_buf_capacity - has_write_size - 1);
	}
	else
	{
		return (m_read_pos - m_write_pos - 1);
	}
}

bool CircleBuffer::CanWrite(int size) const
{
	return (size <= GetCanWriteSize());
}

bool CircleBuffer::Write(const char *buf, int size)
{
	if (!CanWrite(size))
	{
		ELOG("CircleBuffer::Write fail for CanWrite:buf=%p, read_pos=%d, write_pos=%d, can_write_size=%d, size=%d", buf, m_read_pos, m_write_pos, GetCanWriteSize(), size);
		return false;
	}
	// 一定要保证可以写的下！！！

	if (m_read_pos <= m_write_pos)
	{
		// 读位置 在 写位置 前面
		// 0 1 2 3 4 5 6 7 8 9
		// ------r------w----

		// 先写右边空余空间
		int right_can_write_size = m_buf_capacity - m_write_pos;
		int right_write_size = size > right_can_write_size ? right_can_write_size : size;
		memcpy(m_buf + m_write_pos, buf, right_write_size);
		m_write_pos += right_write_size;

		// 如果还有没有写完的，那么肯定可以写在最左边的空余空间
		int reserve_size = size - right_write_size;
		if (reserve_size > 0)
		{
			memcpy(m_buf, buf + right_write_size, reserve_size);
			m_write_pos = reserve_size;
		}
	}
	else
	{
		// 读位置 在 写位置 后面
		// 0 1 2 3 4 5 6 7 8 9
		// ------w------r----

		memcpy(m_buf + m_write_pos, buf, size);
		m_write_pos += size;
	}

	NLOG("CircleBuffer::Write success:buf=%p, size=%d, write_pos=%d", buf, size, m_write_pos);
	return true;
}

int CircleBuffer::GetCanReadSize() const
{
	return (m_buf_capacity - GetCanWriteSize() - 1);
}

bool CircleBuffer::CanRead(int size) const
{
	return (size <= GetCanReadSize());
}

bool CircleBuffer::Read(char *buf, int size, bool change_read_pos/* = true*/)
{
	if (!CanRead(size))
	{
		ELOG("CircleBuffer::Read fail for CanRead:buf=%p, read_pos=%d, write_pos=%d, can_read_size=%d, size=%d", buf, m_read_pos, m_read_pos, GetCanReadSize(), size);
		return false;
	}
	// 一定要保证可以读！！！

	if (m_read_pos <= m_write_pos)
	{
		// 读位置 在 写位置 前面
		// 0 1 2 3 4 5 6 7 8 9
		// ------r------w----

		memcpy(buf, m_buf + m_read_pos, size);
		if (change_read_pos)
		{
			m_read_pos += size;
		}
	}
	else
	{
		// 读位置 在 写位置 后面
		// 0 1 2 3 4 5 6 7 8 9
		// ------w------r----

		// 先读右边
		int right_can_read_size = m_buf_capacity - m_read_pos;
		int right_read_size = size > right_can_read_size ? right_can_read_size : size;
		memcpy(buf, m_buf + m_read_pos, right_read_size);
		if (change_read_pos)
		{
			m_read_pos += right_read_size;
		}

		// 如果还有没有读完的，那么最左边肯定可以读完
		int reserve_size = size - right_read_size;
		if (reserve_size > 0)
		{
			memcpy(buf + right_read_size, m_buf, reserve_size);
			if (change_read_pos)
			{
				m_read_pos = reserve_size;
			}
		}
	}

	NLOG("CircleBuffer::Read success:buf=%p, size=%d, change_read_pos=%d, read_pos=%d", buf, size, change_read_pos, m_read_pos);
	return true;
}

int CircleBuffer::GetPacketDataLength()
{
	if (!CanRead(PACKET_DATA_LENGTH_SIZE))
	{
		return 0;
	}

	char buf[PACKET_DATA_LENGTH_SIZE] = {0};
	if (!Read(buf, PACKET_DATA_LENGTH_SIZE, false))
	{
		return 0;
	}

	return *((uint32_t*)buf);
}

void CircleBuffer::TryDecode()
{
	while (true)
	{
		uint32_t data_length = GetPacketDataLength();
		if (data_length == 0)
		{
			break;
		}

		int length = data_length + PACKET_HEADER_SIZE;
		if (length < GetCanReadSize())
		{
			break;
		}
		bool use_circle = (m_read_pos + length <= m_buf_capacity);

		uint32_t opcode = 0;
		Data server_data;
		std::string server_data_str;
		if (use_circle)
		{
			// 所有可读数据都在右边

			uint32_t temp_data_length = *((uint32_t*)(m_buf + m_read_pos));
			m_read_pos += PACKET_DATA_LENGTH_SIZE;

			opcode = *((uint32_t*)(m_buf + m_read_pos));
			m_read_pos += PACKET_OPCODE_SIZE;

			// 解包
			Packet server_packet(opcode, data_length);
			server_packet.UnpackageData(m_buf + m_read_pos, data_length);
			m_read_pos += data_length;
			server_data.PacketTo(server_packet);

			server_data.GetStr(server_data_str);
		}
		else
		{
			// 可读数据分布在环形缓冲区的最前面和最后面，还是读出来吧！

			char temp_buf[PACKET_MAX_SIZE] = {0};
			Read(temp_buf, length);

			opcode = *((uint32_t*)(temp_buf + PACKET_DATA_LENGTH_SIZE));

			// 解包
			Packet server_packet(opcode, data_length);
			server_packet.UnpackageData(temp_buf + PACKET_HEADER_SIZE, data_length);
			server_data.PacketTo(server_packet);

			server_data.GetStr(server_data_str);
		}
		NLOG("CircleBuffer::TryDecode:opcode=%u, use_circle=%d, data_length=%u, data=[%s]", opcode, use_circle, data_length, server_data_str.c_str());
	}
}
