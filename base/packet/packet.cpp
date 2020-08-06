#include "packet.h"

#include "../debug.h"

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
	DLOG("Packet::PackageData:opcode=%u, data_length=%u", m_opcode, m_data_length);
}

bool Packet::UnpackageData(const char *data_buf, uint32_t data_length)
{
	memcpy(m_data + PACKET_HEADER_SIZE, data_buf, data_length);

	m_data_length = data_length;

	m_pos = PACKET_HEADER_SIZE;
	m_size = PACKET_HEADER_SIZE + m_data_length;

	DLOG("Packet::UnpackageData:opcode=%u, data_length=%u", m_opcode, m_data_length);
	return true;
}
