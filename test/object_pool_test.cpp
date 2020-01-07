#include "object_pool_manager.h"
#include "object.h"

#include <ctime>
#include <thread>
#include <chrono>

/*
* 查看所有心跳对象数量：cut -d':' 1 -f6 | sort -n | uniq | wc -l
* 查看所有心跳对象的心跳数量：cut -d':' 1 -f6 | sort -n | uniq -c
*/


#define TICK_PER_SECOND 10

class Wind : public Object
{
public:
	Wind() {}
	virtual ~Wind() {}

public:
	virtual void Heartbeat()
	{
		NLOG("Wind::Heartbeat:xid=%ld", GetXID().id);
	}
};

class Snow: public Object
{
public:
	Snow() {}
	virtual ~Snow() {}

public:
	virtual void Heartbeat()
	{
		NLOG("Snow::Heartbeat:xid=%ld", GetXID().id);
	}
};

#define WIND_INDEX_MIN 0
#define WIND_INDEX_MAX 100
#define WIND_POOL_SIZE_INIT 10
#define WIND_INC_POOL_SIZE 5
ObjectPoolManager<Wind, TICK_PER_SECOND, WIND_INDEX_MIN, WIND_INDEX_MAX, WIND_INC_POOL_SIZE> opm_wind("Wind");
#define WIND_COUNT 30

#define SNOW_1_INDEX_MIN 0
#define SNOW_1_INDEX_MAX 100
#define SNOW_1_POOL_SIZE_INIT 10
#define SNOW_1_INC_POOL_SIZE 5
ObjectPoolManager<Snow, TICK_PER_SECOND, SNOW_1_INDEX_MIN, SNOW_1_INDEX_MAX, SNOW_1_INC_POOL_SIZE> opm_snow_1("Snow_1");
#define SNOW_1_COUNT 20

#define SNOW_2_INDEX_MIN SNOW_1_INDEX_MAX
#define SNOW_2_INDEX_MAX 200
#define SNOW_2_POOL_SIZE_INIT 10
#define SNOW_2_INC_POOL_SIZE 5
ObjectPoolManager<Snow, TICK_PER_SECOND, SNOW_2_INDEX_MIN, SNOW_2_INDEX_MAX, SNOW_2_INC_POOL_SIZE> opm_snow_2("Snow_2");
#define SNOW_2_COUNT 80

static bool IsValidObject(XID_TYPE xt)
{
	return (xt == XT_WIND || xt == XT_SNOW_1 || xt == XT_SNOW_2);
}
Object* AllocObject(XID_TYPE xt)
{
	if (!IsValidObject(xt))
	{
		return nullptr;
	}

	Object *obj = nullptr;
	switch (xt)
	{
	case XT_WIND:
	{
		Wind *w = opm_wind.Alloc();
		w->SetXID(XID(xt, opm_wind.GetIndex(w)));
		w->Unlock();
		opm_wind.Insert(w);
		obj = w;
	}
	break;
	case XT_SNOW_1:
	{
		Snow *s = opm_snow_1.Alloc();
		s->SetXID(XID(xt, opm_snow_1.GetIndex(s)));
		s->Unlock();
		opm_snow_1.Insert(s);
		obj = s;
	}
	break;
	case XT_SNOW_2:
	{
		Snow *s = opm_snow_2.Alloc();
		s->SetXID(XID(xt, opm_snow_2.GetIndex(s)));
		s->Unlock();
		opm_snow_2.Insert(s);
		obj = s;
	}
	break;
	default:
	break;
	}
	return obj;
}
void FreeObject(Object *obj)
{
	if (!obj)
	{
		assert(false);
		return;
	}
	XID_TYPE xt = obj->GetXID().GetType();
	if (!IsValidObject(xt))
	{
		assert(false);
		return;
	}

	switch (xt)
	{
	case XT_WIND:
	{
		Wind *w = dynamic_cast<Wind*>(obj);
		if (!w)
		{
			assert(false);
			return;
		}
		opm_wind.Remove(w);
		w->Lock();
		opm_wind.Free(w);
		w->Unlock();
	}
	break;
	case XT_SNOW_1:
	{
		Snow *s = dynamic_cast<Snow*>(obj);
		if (!s)
		{
			assert(false);
			return;
		}
		opm_snow_1.Remove(s);
		s->Lock();
		opm_snow_1.Free(s);
		s->Unlock();
	}
	break;
	case XT_SNOW_2:
	{
		Snow *s = dynamic_cast<Snow*>(obj);
		if (!s)
		{
			assert(false);
			return;
		}
		opm_snow_2.Remove(s);
		s->Lock();
		opm_snow_2.Free(s);
		s->Unlock();
	}
	break;
	default:
	break;
	}
}

int main()
{
	opm_wind.Init(WIND_POOL_SIZE_INIT);
	opm_snow_1.Init(SNOW_1_POOL_SIZE_INIT);
	opm_snow_2.Init(SNOW_2_POOL_SIZE_INIT);

	std::thread th([&]{ while(true) { opm_wind.Heartbeat(); opm_snow_1.Heartbeat(); opm_snow_2.Heartbeat(); std::this_thread::sleep_for(std::chrono::seconds(1)); } });
	th.detach();

	Object *obj_wind[WIND_COUNT];
	for (int i = 0; i < WIND_COUNT; ++i)
	{
		obj_wind[i] = AllocObject(XT_WIND);
	}

	Object *obj_snow_1[SNOW_1_COUNT];
	for (int i = 0; i < SNOW_1_COUNT; ++i)
	{
		obj_snow_1[i] = AllocObject(XT_SNOW_1);
	}
	Object *obj_snow_2[SNOW_2_COUNT];
	for (int i = 0; i < SNOW_2_COUNT; ++i)
	{
		obj_snow_2[i] = AllocObject(XT_SNOW_2);
	}

	std::this_thread::sleep_for(std::chrono::seconds(6000));

	for (int i = 0; i < WIND_COUNT; ++i)
	{
		FreeObject(obj_wind[i]);
	}

	for (int i = 0; i < SNOW_1_COUNT; ++i)
	{
		FreeObject(obj_snow_1[i]);
	}
	for (int i = 0; i < SNOW_2_COUNT; ++i)
	{
		FreeObject(obj_snow_2[i]);
	}

	return 0;
}
