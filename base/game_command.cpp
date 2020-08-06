#include "game_command.h"

#include "../protobuf/proto/protobuf_factory.h"
#include "net/net_manager.h"
#include "game_packet.h"
#include "debug.h"

void GameCommand::Command(int session_id, uint32_t opcode, uint32_t data_length, Packet &packet)
{
	switch (opcode)
	{
	case GPO_TEST_DATA:
	{
		TestData data;
		data.PacketTo(packet);

		std::string str;
		data.GetStr(str);
		NLOG("GameCommand::Command:session_id=%d, opcode=%u, data_length=%u, data=[%s]", session_id, opcode, data_length, str.c_str());

		{
			Packet rsp_packet(GPO_TEST_DATA);
			data.ToPacket(rsp_packet);

			SendCommand(session_id, rsp_packet);
		}
	}
	break;

	case GPO_PB_BEGIN ... GPO_PB_END:
	{
		char pb_str[PACKET_MAX_SIZE] = {0};
		packet.PopValue(pb_str, data_length);

		::google::protobuf::Message *message = sProtobufFactory.GetMessage(PT_COMMON, opcode);
		if (!message)
		{
			ELOG("GameCommand::Command fail for pb message:session_id=%d, opcode=%u, data_length=%u", session_id, opcode, data_length);
			break;
		}
		if (!message->ParseFromArray(pb_str, data_length))
		{
			ELOG("GameCommand::Command fail for ParseFromArray:session_id=%d, opcode=%u, data_length=%u", session_id, opcode, data_length);
			break;
		}

		PBCommand(session_id, opcode, message);
	}
	break;

	default:
	{
		ELOG("GameCommand::Command fail for opcode:session_id=%d, opcode=%u", session_id, opcode);
	}
	break;
	}
}

void GameCommand::PBCommand(int session_id, uint32_t opcode, ::google::protobuf::Message *message)
{
	switch (opcode)
	{
	case PB::DT_GET_COMMON_INFO_REQ:
	{
		PB::dt_get_common_info_req *req = dynamic_cast<PB::dt_get_common_info_req*>(message);
		if (!req)
		{
			ELOG("GameCommand::PBCommand fail for dynamic_cast:session_id=%d, opcode=%u", session_id, opcode);
			break;
		}

		NLOG("GameCommand::PBCommand:session_id=%d, opcode=%u, data=[%d]", session_id, opcode, req->id());
		{
			PB::dt_get_common_info_rsp rsp;
			rsp.set_id(req->id());
			PB::common_info *ci = rsp.mutable_oci();
			ci->set_flag(true);
			ci->set_str(std::string("测试optional数据"));
			ci->add_ri(1);
			for (int i = 0; i < 2; ++i)
			{
				PB::common_info* ele = rsp.add_rci();
				ele->set_flag(i / 2 == 0);
				ele->set_str(std::string("测试repeated数据").append(std::to_string(i)));
				ele->add_ri(i);
				ele->add_ri(i + 1);
			}

			//std::string pb_str = rsp.SerializeAsString();
			//uint32_t pb_str_length = pb_str.length();

			//Packet rsp_packet(rsp.type(), pb_str_length);
			//rsp_packet.PushValue(pb_str.c_str(), pb_str_length);

			SendPBCommand(session_id, rsp.type(), &rsp);
		}
	}
	break;

	default:
	{
		ELOG("GameCommand::PBCommand fail for opcode:session_id=%d, opcode=%u", session_id, opcode);
	}
	break;
	}
}

bool GameCommand::SendCommand(int session_id, Packet &packet)
{
	return sNetManager.Send(session_id, packet);
}

bool GameCommand::SendPBCommand(int session_id, uint32_t opcode, ::google::protobuf::Message *message)
{
	if (!message)
	{
		return false;
	}
	//uint32_t opcode = sProtobufFactory.GetType(PT_COMMON, message);
	//if (opcode == 0)
	//{
	//	return false;
	//}

	std::string str = message->SerializeAsString();
	uint32_t str_length = str.length();

	Packet packet(opcode, str_length);
	packet.PushValue(str.c_str(), str_length);

	return sNetManager.Send(session_id, packet);
}
