#pragma once

#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

#include "singleton.h"
#include "packet/packet.h"
#include "../protobuf/cpp/common.pb.h"

class GameCommand : public Singleton<GameCommand>
{
public:
	GameCommand() {}
	virtual ~GameCommand() {}

public:
	// 收到协议，解码
	void Command(int session_id, uint32_t opcode, uint32_t data_length, Packet &packet);
	void PBCommand(int session_id, uint32_t opcode, ::google::protobuf::Message *message);

	// 发送协议
	bool SendCommand(int session_id, Packet &packet);
	bool SendPBCommand(int session_id, uint32_t opcode, ::google::protobuf::Message *message);
};
#define sGameCommand (*GameCommand::Instance())

#endif
