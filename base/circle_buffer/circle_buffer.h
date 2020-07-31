#pragma once

#ifndef CIRCLE_BUFFER_H
#define CIRCLE_BUFFER_H

class CircleBuffer 
{
public:
	explicit CircleBuffer(int buf_capacity);
	virtual ~CircleBuffer();
	CircleBuffer(const CircleBuffer &cb);
	CircleBuffer& operator=(const CircleBuffer &cb);

public:
	int GetCapacity() const { return m_buf_capacity; }

public:
	// m_read_pos左边预留一个空位置，确保 m_read_pos == m_write_pos 时环形缓冲区为空
	int GetCanWriteSize() const;
	bool CanWrite(int size) const;
	bool Write(const char *buf, int size);

	int GetCanReadSize() const;
	bool CanRead(int size) const;
	bool Read(char *buf, int size, bool change_read_pos = true);

public:
	int GetPacketDataLength();

protected:
	char					*m_buf	= nullptr;		// 环形缓冲区
	int						m_buf_capacity = 0;		// 环形缓冲区容量

	int						m_read_pos = 0;
	int						m_write_pos = 0;
};

#endif
