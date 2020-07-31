#include "packet.h"

Packet::Packet(uint32_t opcode)
{
	Init(opcode, PACKET_DEFAULT_SIZE);
}

Packet::Packet(uint32_t opcode, uint32_t data_length)
{
	assert(data_length <= PACKET_MAX_SIZE - PACKET_HEADER_SIZE);

	Init(opcode, PACKET_HEADER_SIZE + data_length);
}

void Packet::Init(uint32_t opcode, uint32_t size)
{
	m_opcode = opcode;
	m_data_length = 0;
	m_pos = PACKET_HEADER_SIZE;

	m_size = size;
	m_data = new char[m_size];
	memset(m_data, 0, m_size);
	memcpy(m_data + PACKET_DATA_LENGTH_SIZE, (char*)&m_opcode, PACKET_OPCODE_SIZE);
}

Packet::~Packet()
{
	if (m_data)
	{
		delete []m_data;
	}
}

void Packet::PackageData()
{
	m_data_length = m_pos - PACKET_HEADER_SIZE;
	memcpy(m_data, (char*)&m_data_length, PACKET_DATA_LENGTH_SIZE);
}

bool Packet::UnpackageData(const char *buf, uint32_t size)
{
	memcpy(m_data, buf, size);

	char data_length_buf[PACKET_DATA_LENGTH_SIZE] = {0};
	memcpy(data_length_buf, m_data, PACKET_DATA_LENGTH_SIZE);
	m_data_length = *((uint32_t*)data_length_buf);

	char opcode_buf[PACKET_OPCODE_SIZE] = {0};
	memcpy(opcode_buf, m_data + PACKET_DATA_LENGTH_SIZE, PACKET_OPCODE_SIZE);
	m_opcode = *((uint32_t*)opcode_buf);

	m_pos = PACKET_HEADER_SIZE;
	m_size = PACKET_HEADER_SIZE + m_data_length;

	return (m_data_length + PACKET_HEADER_SIZE == size);
}
