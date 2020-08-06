#include "protobuf_factory.h"

#include <sstream>

#include "../../base/debug.h"

static bool IsValidProtobufType(uint32_t type)
{
	return (type >= PT_COMMON && type < PT_COUNT);
}

ProtobufFactory::ProtobufFactory()
{
	m_protobuf_conf.insert(std::make_pair(PT_COMMON, std::make_pair("common.proto", "PB.DATA_TYPE")));
}

ProtobufFactory::~ProtobufFactory()
{
	m_protobuf_conf.clear();
	m_protobuf_type.clear();

	for (int i = PT_BEGIN; i < PT_COUNT; ++i)
	{
		for (auto it = m_protobuf_message[i].begin(); it != m_protobuf_message[i].end(); ++it)
		{
			delete it->second;
		}
		m_protobuf_message[i].clear();
	}
}

bool ProtobufFactory::Init()
{
	for (auto it = m_protobuf_conf.begin(); it != m_protobuf_conf.end(); ++it)
	{
		std::string protobuf_file_name = std::get<0>(it->second);
		std::string protobuf_enum_name = std::get<1>(it->second);

		if (protobuf_file_name.empty() || protobuf_enum_name.empty())
		{
			ELOG("ProtobufFactory::Init fail for empty:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str());
			return false;
		}

		const ::google::protobuf::DescriptorPool *descriptor_pool = ::google::protobuf::DescriptorPool::generated_pool();
		if (!descriptor_pool)
		{
			ELOG("ProtobufFactory::Init fail for DescriptorPool:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str());
			return false;
		}

		const ::google::protobuf::FileDescriptor *file_descriptor = descriptor_pool->FindFileByName(protobuf_file_name);
		if (!file_descriptor)
		{
			ELOG("ProtobufFactory::Init fail for FileDescriptor:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str());
			return false;
		}

		const ::google::protobuf::EnumDescriptor *enum_descriptor = descriptor_pool->FindEnumTypeByName(protobuf_enum_name);
		if (!enum_descriptor)
		{
			ELOG("ProtobufFactory::Init fail for EnumDescriptor:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str());
			return false;
		}

		std::stringstream type_str;
		for(int i = 0; i < file_descriptor->message_type_count(); ++i)
		{
			const ::google::protobuf::Descriptor *descriptor = file_descriptor->message_type(i);
			if (!descriptor)
			{
				ELOG("ProtobufFactory::Init fail for Descriptor:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s, message_type_count=%d", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str(), i);
				return false;
			}

			const ::google::protobuf::FieldDescriptor *fd = descriptor->FindFieldByNumber(1);
			if (!fd || fd->enum_type() != enum_descriptor)
			{
				continue;
			}

			uint32_t type = fd->default_value_enum()->number();
			if (m_protobuf_message[it->first].find(type) != m_protobuf_message[it->first].end())
			{
				ELOG("ProtobufFactory::Init fail for duplicate type:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s, type=%u", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str(), type);
				return false;
			}
			type_str << type << ", ";

			::google::protobuf::Message *message = ::google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor)->New();
			m_protobuf_type.insert(std::make_pair(descriptor, std::make_tuple(it->first, type)));
			m_protobuf_message[it->first].insert(std::make_pair(type, message));
		}
		DLOG("ProtobufFactory::Init success:protobuf_type=%d, protobuf_file_name=%s, protobuf_enum_name=%s, type_str=<%s>", it->first, protobuf_file_name.c_str(), protobuf_enum_name.c_str(), type_str.str().c_str());
	}

	return true;
}

uint32_t ProtobufFactory::GetType(PROTOBUF_TYPE pt, const ::google::protobuf::Message *message) const
{
	if (!message)
	{
		return 0;
	}

	auto it = m_protobuf_type.find(message->GetDescriptor());
	if (it == m_protobuf_type.end())
	{
		return 0;
	}

	if (std::get<0>(it->second) == pt)
	{
		return std::get<1>(it->second);
	}
}

uint32_t ProtobufFactory::GetType(PROTOBUF_TYPE pt, const ::google::protobuf::Message &message) const
{
	return GetType(pt, &message);
}

::google::protobuf::Message* ProtobufFactory::GetMessage(PROTOBUF_TYPE pt, uint32_t type)
{
	if (!IsValidProtobufType(pt) || type == 0)
	{
		return nullptr;
	}

	auto it = m_protobuf_message[pt].find(type);
	if (it == m_protobuf_message[pt].end())
	{
		return nullptr;
	}

	return it->second;
}
