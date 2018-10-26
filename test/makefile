
all:log4cplus_test singleton_test thread_pool_test protobuf_test thread_safe_container_test packet_test
.PHONY:all

LIB_OPTION_COMMON=-std=c++11 -g

LIB_OPTION_LOG4CPLUS=-I../base/log -I../third_party_lib/log4cplus/include -L../third_party_lib/log4cplus/.libs -Wl,-rpath ../third_party_lib/log4cplus/.libs -llog4cplus
FILE_LIST_LOG4CPLUS=./log4cplus_test.cpp ../base/log/log.cpp
log4cplus_test:$(FILE_LIST_LOG4CPLUS)
	g++ $(FILE_LIST_LOG4CPLUS) -o log4cplus_test $(LIB_OPTION_COMMON) $(LIB_OPTION_LOG4CPLUS)

LIB_OPTION_SINGLETON=-I../base
FILE_LIST_SINGLETON=./singleton_test.cpp
singleton_test:$(FILE_LIST_SINGLETON)
	g++ $(FILE_LIST_SINGLETON) -o singleton_test $(LIB_OPTION_COMMON) $(LIB_OPTION_SINGLETON)

LIB_OPTION_THREAD_POOL=-I../base -I../base/thread_pool -lpthread
FILE_LIST_THREAD_POOL=./thread_pool_test.cpp ../base/thread_pool/thread_pool.cpp ../base/thread_pool/worker.cpp ../base/log/log.cpp
thread_pool_test:$(FILE_LIST_THREAD_POOL)
	g++ $(FILE_LIST_THREAD_POOL) -o thread_pool_test $(LIB_OPTION_COMMON) $(LIB_OPTION_THREAD_POOL) $(LIB_OPTION_LOG4CPLUS)

LIB_OPTION_PROTOBUF=-I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf -pthread
FILE_LIST_PROTOBUF=./protobuf_test.cpp ../protobuf/cpp/*
protobuf_test:$(FILE_LIST_PROTOBUF)
	g++ $(FILE_LIST_PROTOBUF) -o protobuf_test $(LIB_OPTION_COMMON) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_THREAD_SAFE_CONTAINER=-I../base/thread_safe_container
FILE_LIST_THREAD_SAFE_CONTAINER=./thread_safe_container_test.cpp
thread_safe_container_test:$(FILE_LIST_THREAD_SAFE_CONTAINER)
	g++ $(FILE_LIST_THREAD_SAFE_CONTAINER) -o thread_safe_container_test $(LIB_OPTION_COMMON) $(LIB_OPTION_THREAD_SAFE_CONTAINER)

LIB_OPTION_PACKET=-I../base/packet/
FILE_LIST_PACKET=./packet_test.cpp ../base/packet/packet.cpp ../protobuf/cpp/*
packet_test:$(FILE_LIST_PACKET)
	g++ $(FILE_LIST_PACKET) -o packet_test $(LIB_OPTION_COMMON) $(LIB_OPTION_PACKET) $(LIB_OPTION_PROTOBUF)

.PHONY:clean
clean:
	rm -f log4cplus_test
	rm -f singleton_test
	rm -f thread_pool_test
	rm -f protobuf_test
	rm -f thread_safe_container_test
	rm -f packet_test
