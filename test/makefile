
all:common_test log4cplus_test singleton_test thread_pool_test protobuf_test thread_safe_container_test packet_test object_pool_test timer_test shm_test patch_test
.PHONY:all

LIB_OPTION_BASE=-std=c++11 -g

LIB_OPTION_COMMON=-I../base
FILE_LIST_COMMON=./common_test.cpp
common_test:$(FILE_LIST_COMMON)
	g++ $(FILE_LIST_COMMON) -o common_test $(LIB_OPTION_BASE) $(LIB_OPTION_COMMON)

LIB_OPTION_LOG4CPLUS=-I../base/log -I../third_party_lib/log4cplus/include -L../third_party_lib/log4cplus/.libs -Wl,-rpath ../third_party_lib/log4cplus/.libs -llog4cplus -pthread
FILE_LIST_LOG4CPLUS=./log4cplus_test.cpp ../base/log/log.cpp
log4cplus_test:$(FILE_LIST_LOG4CPLUS)
	g++ $(FILE_LIST_LOG4CPLUS) -o log4cplus_test $(LIB_OPTION_BASE) $(LIB_OPTION_LOG4CPLUS)

LIB_OPTION_SINGLETON=-I../base
FILE_LIST_SINGLETON=./singleton_test.cpp
singleton_test:$(FILE_LIST_SINGLETON)
	g++ $(FILE_LIST_SINGLETON) -o singleton_test $(LIB_OPTION_BASE) $(LIB_OPTION_SINGLETON)

LIB_OPTION_THREAD_POOL=-I../base -I../base/thread_pool -lpthread
FILE_LIST_THREAD_POOL=./thread_pool_test.cpp ../base/thread_pool/thread_pool.cpp ../base/thread_pool/worker.cpp
thread_pool_test:$(FILE_LIST_THREAD_POOL)
	g++ $(FILE_LIST_THREAD_POOL) -o thread_pool_test $(LIB_OPTION_BASE) $(LIB_OPTION_THREAD_POOL)

LIB_OPTION_PROTOBUF=-I/usr/local/include/google/protobuf -I../base -L/usr/local/lib -lprotobuf -Wl,-rpath /usr/local/lib -pthread
FILE_LIST_PROTOBUF=./protobuf_test.cpp ../protobuf/cpp/*
protobuf_test:$(FILE_LIST_PROTOBUF)
	g++ $(FILE_LIST_PROTOBUF) -o protobuf_test $(LIB_OPTION_BASE) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_THREAD_SAFE_CONTAINER=-I../base -I../base/thread_safe_container
FILE_LIST_THREAD_SAFE_CONTAINER=./thread_safe_container_test.cpp
thread_safe_container_test:$(FILE_LIST_THREAD_SAFE_CONTAINER)
	g++ $(FILE_LIST_THREAD_SAFE_CONTAINER) -o thread_safe_container_test $(LIB_OPTION_BASE) $(LIB_OPTION_THREAD_SAFE_CONTAINER)

LIB_OPTION_PACKET=-I../base/packet/
FILE_LIST_PACKET=./packet_test.cpp ../base/packet/packet.cpp ../protobuf/cpp/*
packet_test:$(FILE_LIST_PACKET)
	g++ $(FILE_LIST_PACKET) -o packet_test $(LIB_OPTION_BASE) $(LIB_OPTION_PACKET) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_OBJECT_POOL=-I../base -I../base/object_pool -pthread
FILE_LIST_OBJECT_POOL=./object_pool_test.cpp
object_pool_test:$(FILE_LIST_OBJECT_POOL)
	g++ $(FILE_LIST_OBJECT_POOL) -o object_pool_test $(LIB_OPTION_BASE) $(LIB_OPTION_OBJECT_POOL)

LIB_OPTION_TIMER=-I../base -I../base/timer -pthread
FILE_LIST_TIMER=./timer_test.cpp
timer_test:$(FILE_LIST_TIMER)
	g++ $(FILE_LIST_TIMER) -o timer_test $(LIB_OPTION_BASE) $(LIB_OPTION_TIMER)

LIB_OPTION_SHM=-I../base/shm -pthread
FILE_LIST_SHM_MASTER=./shm_master_test.cpp
FILE_LIST_SHM_SLAVE=./shm_slave_test.cpp
shm_test:$(FILE_LIST_SHM_MASTER) $(FILE_LIST_SHM_SLAVE)
	g++ $(FILE_LIST_SHM_MASTER) -o shm_master_test $(LIB_OPTION_BASE) $(LIB_OPTION_SHM)
	g++ $(FILE_LIST_SHM_SLAVE) -o shm_slave_test $(LIB_OPTION_BASE) $(LIB_OPTION_SHM)

LIB_OPTION_PATCH=-I../base -pthread -L. -Wl,-rpath . -fno-access-control -rdynamic -fpic -ldl -m64
FILE_LIST_PATCH=./patch_test.cpp ./patch_wind.cpp
patch_test:$(FILE_LIST_PATCH)
	g++ $(FILE_LIST_PATCH) -o patch_test $(LIB_OPTION_BASE) $(LIB_OPTION_PATCH)

.PHONY:clean
clean:
	/usr/bin/rm -f common_test
	/usr/bin/rm -f log4cplus_test
	/usr/bin/rm -f singleton_test
	/usr/bin/rm -f thread_pool_test
	/usr/bin/rm -f protobuf_test
	/usr/bin/rm -f thread_safe_container_test
	/usr/bin/rm -f packet_test
	/usr/bin/rm -f object_pool_test
	/usr/bin/rm -f timer_test
	/usr/bin/rm -f shm_master_test
	/usr/bin/rm -f shm_slave_test
	/usr/bin/rm -f patch_test
	/usr/bin/rm -f libpatch.so
