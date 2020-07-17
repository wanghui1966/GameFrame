#pragma once

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <functional>

// RAII类，在函数入口处定义一个回调函数，在函数退出时RAII可以保证一定会执行这个回调函数
class ScopeGuard
{
public:
	explicit ScopeGuard(std::function<void()> cb) : m_cb(cb) {}
	~ScopeGuard()
	{
		m_cb();
	}

private:
	ScopeGuard(const ScopeGuard&) = delete;
	ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
	std::function<void()> m_cb;
};
#define SCOPE_GUARD(cb) ScopeGuard sg(cb)

#endif
