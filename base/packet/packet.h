#ifndef PACKET_H
#define PACKET_H

#include <cstring>		// memset
#include <string>		// string
#include <cassert>		// assert

/*

   任何数据系列化后都会变成：数据=数据头(操作码+数据体长度)+数据体

   任何数据结构系列化后都会变成一个char类型的数组

*/

// 操作码 长度(字节)
const uint32_t PACKET_OPCODE_SIZE = 4;
// 数据体长度 长度(字节)
const uint32_t PACKET_DATA_LENGTH_SIZE = 4;
// 数据头 长度(字节)
const uint32_t PACKET_HEADER_SIZE = PACKET_OPCODE_SIZE + PACKET_DATA_LENGTH_SIZE;
// 数据 默认长度(字节)
const uint32_t PACKET_DEFAULT_SIZE = 1024;
// 数据 最大长度(字节)
const uint32_t PACKET_MAX_SIZE = 8196;

// 系列化类
class Packet
{
public:
	// 提供给预先不知道数据体大小的结构初始化用，比如结构体
	Packet(uint32_t opcode);
	// 提供给预先知道数据体大小的结构初始化用，比如PB
	Packet(uint32_t opcode, uint32_t data_length);
	virtual ~Packet();

protected:
	void Init(uint32_t opcode, uint32_t size);

public:
	void PackageData();
	bool UnpackageData(const char *buf, uint32_t size);

	inline uint32_t GetOpcode() { return m_opcode; }
	inline uint32_t GetDataLength() { return m_data_length; }
	inline uint32_t GetSize() { return (PACKET_HEADER_SIZE + m_data_length); }
	inline char* GetData() { return m_data; }

// 重载输出输入操作符
public:
	#define DECLARE_OPERATOR_OVERRIDE(DATA_TYPE) \
		Packet& operator<<(DATA_TYPE value) { WriteData<DATA_TYPE>(value); return *this; } \
		Packet& operator>>(DATA_TYPE &value) { ReadData<DATA_TYPE>(value); return *this; }

	// 基本数据类型重载
	DECLARE_OPERATOR_OVERRIDE(char)
	DECLARE_OPERATOR_OVERRIDE(unsigned char)
	DECLARE_OPERATOR_OVERRIDE(bool)
	DECLARE_OPERATOR_OVERRIDE(short)
	DECLARE_OPERATOR_OVERRIDE(unsigned short)
	DECLARE_OPERATOR_OVERRIDE(int32_t)
	DECLARE_OPERATOR_OVERRIDE(uint32_t)
	DECLARE_OPERATOR_OVERRIDE(int64_t)
	DECLARE_OPERATOR_OVERRIDE(uint64_t)
	DECLARE_OPERATOR_OVERRIDE(float)
	DECLARE_OPERATOR_OVERRIDE(double)

	// std::string重载，先读/写长度，再读/写数据
	Packet& operator<<(std::string &value)
	{
		uint32_t length = value.length();
		WriteData<uint32_t>(length);
		WriteData((const char*)value.c_str(), length);
	}
	Packet& operator>>(std::string &value)
	{
		uint32_t length = 0;
		ReadData<uint32_t>(length);

		if (length > 0)
		{
			char *buf = new char[length];
			memset(buf, 0, length);
			ReadData(buf, length);
			value = buf;

			delete []buf;
			buf = nullptr;
		}
		return *this;
	}

	// 给char*用
	void PushValue(const char *value, uint32_t size)
	{
		WriteData(value, size);
	}
	void PopValue(char *value, uint32_t size)
	{
		ReadData(value, size);
	}

// 读写数据
protected:
	void Resize(uint32_t size)
	{
		assert(m_pos < PACKET_MAX_SIZE - PACKET_HEADER_SIZE);
		if (m_pos + size <= m_size)
		{
			return;
		}

		char *new_data = new char[m_pos + size];
		memset(new_data, 0, m_pos + size);
		memcpy(new_data, m_data, m_pos - 1);

		delete []m_data;
		m_data = new_data;
		m_size = m_pos + size;
	}

	template<typename T>
	void WriteData(const T& value)
	{
		WriteData((const char*)&value, sizeof(T));
	}
	void WriteData(const char *value, uint32_t size)
	{
		Resize(size);
		memcpy(m_data + m_pos, value, size);
		m_pos += size;
	}
	template<typename T>
	void ReadData(T &value)
	{
		ReadData((char*)&value, sizeof(T));
	}
	void ReadData(char *value, uint32_t size)
	{
		memcpy(value, m_data + m_pos, size);
		m_pos += size;
	}

protected:
	char*				m_data;							// 数据指针
	uint32_t			m_size;							// 数据长度

	uint32_t			m_opcode;						// 操作码
	uint32_t			m_data_length;					// 数据长度

	uint32_t			m_pos;							// 数据读/写的偏移
};

#endif
