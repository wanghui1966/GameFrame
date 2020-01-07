#pragma once

#ifndef SHM_MANAGER_H
#define SHM_MANAGER_H

#include "../singleton.h"
#include "shm_data.h"

#include "../debug.h"

#include <sys/shm.h>
#include <cstring>

/*
 * 查看当前系统共享内存信息：ipcs -m
 * 查看当前系统共享内存当前状态：ipcs -u
 * 查看当前系统共享内存限制：ipcs -m -l
 * 查看当前系统共享内存全线：ipcs -m -c
 * 查看当前系统共享内存最后访问过的进程：ipcs -m -p
 * 查看当前系统共享内存最后操作时间：ipcs -m -t
 * 删除当前系统共享内存：ipcrm -m shmid
*/

// 共享内存的键值
const key_t SHM_KEY = 6666;
// 共享内存大小
const size_t SHM_DEFAULT_SIZE = 1 * 1024 * 1024;

// 共享内存管理器，单例类
class ShmManager : public Singleton<ShmManager>
{
public:
	ShmManager() {}
	~ShmManager() {}

public:
	// 创建共享内存，创建共享内存进程调用
	bool Create(size_t size = SHM_DEFAULT_SIZE)
	{
		size = size > 0 ? size : SHM_DEFAULT_SIZE;

		int m_id = shmget(SHM_KEY, size, IPC_CREAT | 0666);
		if (m_id == -1)
		{
			if (errno == EINVAL || errno == ENOSPC || errno == ENOENT || errno == EACCES || errno == ENOMEM)
			{
				ELOG("ShmManager Create:errno=%d", errno);
				return false;
			}
			else if (errno == EEXIST)
			{
				Free();
			}
		}

		m_data = (void*)shmat(m_id, 0, 0);
		if (m_data == (void*)-1)
		{
			return false;
		}
		NLOG("ShmManager::Create");

		ShmData *sd = (ShmData*)m_data;
		memset(sd, 0, sizeof(ShmData));

		return true;
	}

	// 访问共享内存，除创建共享内存进程外的其他进程调用
	bool Attach()
	{
		m_id = shmget(SHM_KEY, 0, 0);
		if (m_id == -1)
		{
			ELOG("ShmManager::Attach fail for shmget");
			return false;
		}

		m_data = (void*)shmat(m_id, 0, 0);
		if (m_data == (void*)-1)
		{
			ELOG("ShmManager::Attach fail for shmat");
			return false;
		}

		NLOG("ShmManager::Attach");
		return true;
	}

	// 断开共享内存，使用完毕后回收，所有进程都必须调用
	bool Deattach()
	{
		shmdt(m_data);
		NLOG("ShmManager::Deattach");
	}

	// 释放共享内存，创建共享内存进程使用
	bool Free()
	{
		struct shmid_ds buf;
		shmctl(m_id, IPC_RMID, &buf);
		NLOG("ShmManager::Free");
	}

public:
	ShmData* GetShmData()
	{
		return (ShmData*)m_data;
	}

protected:
	int				m_id = 0;
	void			*m_data = nullptr;		// 首地址
};
#define sShmManager (*ShmManager::Instance())

#endif
