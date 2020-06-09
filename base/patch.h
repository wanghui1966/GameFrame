#pragma once

#ifndef PATCH_H
#define PATCH_H

#include <sys/stat.h>
#include <dlfcn.h>
#include <sys/user.h>
#include <sys/mman.h>

#include <string>
#include <cstring>
#include <vector>

#include "singleton.h"
#include "debug.h"

typedef std::vector<std::string> FUNC_NAME_LIST;
typedef void (*PATCH_INFO_FUNC)(FUNC_NAME_LIST &func_list);

class Patch : public Singleton<Patch>
{
public:
	Patch() {}
	virtual ~Patch()
	{
		if (m_handle)
		{
			dlclose(m_handle);
			m_handle = nullptr;
		}
	}

public:
	void Init(const std::string &file_name, int interval = 5)
	{
		m_file_name = file_name;
		m_interval = 5;
		NLOG("Patch::Init success:file_name=%s, interval=%d", m_file_name.c_str(), interval);
	}

	void Update()
	{
		time_t now = time(nullptr);
		if (m_last_update_time > 0 && now - m_last_update_time < m_interval)
		{
			return;
		}

		struct stat st;
		memset(&st, 0, sizeof(st));
		if (stat(m_file_name.c_str(), &st) == -1)
		{
			return;
		}
		if (st.st_size <= 0)
		{
			return;
		}

		if (st.st_mtime == m_file_stat_time && st.st_size == m_file_stat_size)
		{
			return;
		}
		m_file_stat_time = st.st_mtime;
		m_file_stat_size = st.st_size;

		if (m_handle)
		{
			dlclose(m_handle);
			m_handle = nullptr;
		}
		m_handle = dlopen(m_file_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (m_handle == nullptr)
		{
			ELOG("Patch::Update fail for dlopen:file_name=%s, err_str=%s", m_file_name.c_str(), dlerror());
			return;
		}

		// 获取热更函数列表
		PATCH_INFO_FUNC get_patch_func_name = (PATCH_INFO_FUNC)dlsym(m_handle, "get_patch_func_name");
		if (get_patch_func_name == nullptr)
		{
			ELOG("Patch::Update fail for get_patch_func_name:file_name=%s, err_str=%s", m_file_name.c_str(), dlerror());
			dlclose(m_handle);
			m_handle = nullptr;
			return;
		}

		FUNC_NAME_LIST func_name_list;
		get_patch_func_name(func_name_list);
		if (func_name_list.empty())
		{
			ELOG("Patch::Update fail for empty func_name_list:file_name=%s", m_file_name.c_str());
			dlclose(m_handle);
			m_handle = nullptr;
			return;
		}

		for (auto it = func_name_list.begin(); it != func_name_list.end(); ++it)
		{
			void *old_func_addr = nullptr;
			if (!GetFuncAddr(nullptr, *it, old_func_addr) || old_func_addr == nullptr)
			{
				continue;
			}

			void *new_func_addr = nullptr;
			if (!GetFuncAddr(m_handle, *it, new_func_addr) || new_func_addr == nullptr)
			{
				continue;
			}

			if (old_func_addr == new_func_addr)
			{
				ELOG("Patch::Update fail for no need patch:file_name=%s, func_name=%s", m_file_name.c_str(), it->c_str());
				continue;
			}
			NLOG("Patch::Update:file_name=%s, func_name=%s, old_func_addr=%p, new_func_addr=%p", m_file_name.c_str(), it->c_str(), old_func_addr, new_func_addr);

			// move $new_func_addr %rax
			// jmp %rax
			unsigned char buf[12] = { 0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0 };

			memcpy(&buf[2], &new_func_addr, sizeof(void*));
			void *page_align = (void*)((uintptr_t)old_func_addr & PAGE_MASK);
			int pages = ((char*)old_func_addr + 12) > (char*)page_align + PAGE_SIZE ? 2 : 1;//是否跨了两页
			mprotect(page_align, PAGE_SIZE * pages, PROT_READ | PROT_WRITE | PROT_EXEC);
			memcpy(old_func_addr, buf, sizeof(buf));
		}
	}

protected:
	bool GetFuncAddr(void *handle, const std::string &func_name, void *&func_addr)
	{
		func_addr = dlsym(handle ? handle : RTLD_DEFAULT, func_name.c_str());
		const char *err = dlerror();
		if (err)
		{
			ELOG("Patch::GetFuncAddr fail for dlsym:file_name=%s, func_name=%s, err_str=%s", m_file_name.c_str(), func_name.c_str(), err);
			return false;
		}
		return true;
	}

protected:
	std::string m_file_name;			// 热更so文件名，当前目录
	void *m_handle = nullptr;			// 热更handle
	int m_interval = 0;					// 热更间隔时间
	time_t m_last_update_time = 0;		// 上次热更时间
	time_t m_file_stat_time = 0;		// 热更文件的最近修改时间
	off_t m_file_stat_size = 0;			// 热更文件大小
};
#define sPatch (*Patch::Instance())

#endif
