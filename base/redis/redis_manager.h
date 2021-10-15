#pragma once

#ifndef REDIS_MANAGER_H
#define REDIS_MANAGER_H

#include "../../third_party_lib/hiredis/hiredis.h"
#include "../singleton.h"

#include <string>
#include <vector>
#include <tuple>

#include <mutex>

class RedisReply
{
public:
	RedisReply() {}
	RedisReply(redisReply *redis_reply);
	virtual ~RedisReply();

public:
	void SetRedisReply(redisReply *redis_reply);
	redisReply* GetReply() const;

public:
	bool IsArray() const;
	bool IsString() const;
	bool IsInteger() const;
	bool IsNil() const;
	bool IsStatus() const;
	bool IsError() const;

public:
	int GetType() const;
	bool GetArrayStrings(std::vector<std::string> &datas) const;
	bool GetString(std::string &data) const;
	bool GetInteger(long long &data) const;
	bool GetStatus(std::string &data) const;
	bool GetError(std::string &data) const;

protected:
	redisReply			*m_redis_reply = nullptr;
};

class Redis
{
public:
	Redis() {}
	virtual ~Redis();
	void SetIndex(int index) { m_index = index; }
	int GetIndex() { return m_index; }

public:
	bool Init(const std::string &host, uint32_t port, const std::string &password);
	bool Connect();
	bool Connect(const std::string &host, uint32_t port, const std::string &password);
	bool ReConnect();
	bool EnsureConnect();

public:
	bool ExecuteCommand(RedisReply *reply, const char *format, ...);
	bool ExecuteCommand(RedisReply &reply, const char *format, ...);
	bool ExecuteCommandArgv(RedisReply *reply, int argc, const char **argv, const size_t *argvlen);
	bool ExecuteCommandArgv(RedisReply &reply, int argc, const char **argv, const size_t *argvlen);

	bool ExecuteAppendCommand(const char *format, ...);
	bool ExecuteAppendCommandArgv(int argc, const char **argv, const size_t *argvlen);

protected:
	bool ExecuteCommandV(RedisReply *reply, const char *format, va_list ap);

protected:
	int						m_index = 0;				// 唯一标识
	redisContext			*m_connect = nullptr;		// 连接

	std::string				m_host = "127.0.0.1";		// redis地址
	uint32_t				m_port = 6379;				// redis端口
	std::string				m_password;					// 认证密码
};

class RedisManager : public Singleton<RedisManager>
{
public:
	RedisManager() {}
	virtual ~RedisManager();

public:
	bool Init(const std::string &host, uint32_t port, const std::string &password, int pool_init_count = 3, int pool_max_count = 10);
	Redis* Get();
	void Put(Redis *redis);

public:
	bool ExecuteCommand(RedisReply *reply, const char *format, ...);
	bool ExecuteCommand(RedisReply &reply, const char *format, ...);

protected:
	bool AppendPool(int inc_count);

protected:
	std::mutex								m_mutex;				// 新建连接的时候保护连接池
	std::vector<std::tuple<bool, Redis*>>	m_pool;					// 连接池，<is_free, Redis>
	int										m_pool_init_count = 3;	// 连接池初始
	int										m_pool_max_count = 10;	// 连接池最大数量
	int										m_pool_inc_count = 1;	// 连接池每次扩充数量

	std::string								m_host = "127.0.0.1";	// redis地址
	uint32_t								m_port = 6379;			// redis端口
	std::string								m_password;				// 认证密码
};
#define sRedisManager (*RedisManager::Instance())

#endif
