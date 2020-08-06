#pragma once

#ifndef PROTOBUF_FACTORY_H
#define PROTOBUF_FACTORY_H

#include <google/protobuf/message.h>

#include <string>
#include <tuple>
#include <map>

#include "../../base/singleton.h"

enum PROTOBUF_TYPE
{
	PT_BEGIN			= 0,

	PT_COMMON			= 0,

	PT_COUNT,
};

class ProtobufFactory : public Singleton<ProtobufFactory>
{
public:
	ProtobufFactory();
	virtual ~ProtobufFactory();

public:
	// 初始化
	bool Init();

	// 由命名空间和消息 获取类型
	uint32_t GetType(PROTOBUF_TYPE pt, const ::google::protobuf::Message *message) const;
	uint32_t GetType(PROTOBUF_TYPE pt, const ::google::protobuf::Message &message) const;

	// 由命名空间和类型获取 消息
	::google::protobuf::Message* GetMessage(PROTOBUF_TYPE pt, uint32_t type);

protected:
	typedef std::map<PROTOBUF_TYPE, std::tuple<std::string, std::string>> PROTOBUF_CONF_MAP;
	typedef std::map<const ::google::protobuf::Descriptor*, std::tuple<PROTOBUF_TYPE, uint32_t>> PROTOBUF_TYPE_MAP;
	typedef std::map<uint32_t, ::google::protobuf::Message*> PROTOBUF_MESSAGE_MAP;

	PROTOBUF_CONF_MAP			m_protobuf_conf;
	PROTOBUF_TYPE_MAP			m_protobuf_type;
	PROTOBUF_MESSAGE_MAP		m_protobuf_message[PT_COUNT];
};
#define sProtobufFactory (*ProtobufFactory::Instance())

#endif
