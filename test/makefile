
all:singleton_test thread_pool_test protobuf_test
.PHONY:all

LIB_OPTION_SINGLETON=-I../base -std=c++11 -g
FILE_LIST_SINGLETON=singleton_test.cpp
singleton_test:$(FILE_LIST_SINGLETON)
	g++ $(FILE_LIST_SINGLETON) -o singleton_test $(LIB_OPTION_SINGLETON)

LIB_OPTION_THREAD_POOL=-I../base -std=c++11 -lpthread -g
FILE_LIST_THREAD_POOL=thread_pool_test.cpp ../base/thread_pool.cpp ../base/worker.cpp
thread_pool_test:$(FILE_LIST_THREAD_POOL)
	g++ $(FILE_LIST_THREAD_POOL) -o thread_pool_test $(LIB_OPTION_THREAD_POOL)

LIB_OPTION_PROTOBUF=-g -std=c++11 -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf -pthread
FILE_LIST_PROTOBUF=./protobuf_test.cpp ../protobuf/cpp/*
protobuf_test:$(FILE_LIST_PROTOBUF)
	g++ $(FILE_LIST_PROTOBUF) -o protobuf_test $(LIB_OPTION_PROTOBUF)

.PHONY:clean
clean:
	rm -f singleton_test
	rm -f thread_pool_test
	rm -f protobuf_test
