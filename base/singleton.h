#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

template <class T> 
class Singleton
{
public:
    static T* NewInstance()
    {
        return new T();
    }

    static void DeleteInstance()
    {
        delete s_instance;
        s_instance = 0;
    }

    static T* Instance()
    {
        return s_instance;
    }

protected:
    // 禁止在栈上创建
    Singleton()
    {
        s_instance = (T*)this;
    }

    virtual ~Singleton()
    {
        s_instance = nullptr;
    }

private:
    // 禁止拷贝构造函数
    Singleton(const Singleton& s) {}

protected:
    static T* s_instance;
};

template<class T>
T* Singleton<T>::s_instance = nullptr;

#endif