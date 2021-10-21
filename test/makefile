
#all:common_test log4cplus_test singleton_test thread_pool_test protobuf_test thread_safe_container_test packet_test object_pool_test timer_test shm_test patch_test redis_test circle_buffer_test net_test snow_flake_test redis_lock_test redis_script_test
all:redis_script_test
.PHONY:all

LIB_OPTION_BASE=-std=c++11 -g -I../base -D__NO_DEBUG_LOG

LIB_OPTION_COMMON=
FILE_LIST_COMMON=./common_test.cpp
common_test:$(FILE_LIST_COMMON)
	g++ $(FILE_LIST_COMMON) -o common_test $(LIB_OPTION_BASE) $(LIB_OPTION_COMMON)

LIB_OPTION_LOG4CPLUS=-I../base/log -I../third_party_lib/log4cplus/include -L../third_party_lib/log4cplus/.libs -Wl,-rpath ../third_party_lib/log4cplus/.libs -llog4cplus -pthread
FILE_LIST_LOG4CPLUS=./log4cplus_test.cpp ../base/log/log.cpp
log4cplus_test:$(FILE_LIST_LOG4CPLUS)
	g++ $(FILE_LIST_LOG4CPLUS) -o log4cplus_test $(LIB_OPTION_BASE) $(LIB_OPTION_LOG4CPLUS)

LIB_OPTION_SINGLETON=
FILE_LIST_SINGLETON=./singleton_test.cpp
singleton_test:$(FILE_LIST_SINGLETON)
	g++ $(FILE_LIST_SINGLETON) -o singleton_test $(LIB_OPTION_BASE) $(LIB_OPTION_SINGLETON)

LIB_OPTION_THREAD_POOL=-I../base/thread_pool -lpthread
FILE_LIST_THREAD_POOL=./thread_pool_test.cpp ../base/thread_pool/thread_pool.cpp ../base/thread_pool/worker.cpp
thread_pool_test:$(FILE_LIST_THREAD_POOL)
	g++ $(FILE_LIST_THREAD_POOL) -o thread_pool_test $(LIB_OPTION_BASE) $(LIB_OPTION_THREAD_POOL)

LIB_OPTION_PROTOBUF=-I/usr/local/include/google/protobuf -I../protobuf/proto -I../protobuf/cpp -L/usr/local/lib -lprotobuf -Wl,-rpath /usr/local/lib -pthread
FILE_LIST_PROTOBUF=./protobuf_test.cpp ../protobuf/cpp/* ../protobuf/proto/protobuf_factory.cpp
protobuf_test:$(FILE_LIST_PROTOBUF)
	g++ $(FILE_LIST_PROTOBUF) -o protobuf_test $(LIB_OPTION_BASE) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_THREAD_SAFE_CONTAINER=-I../base/thread_safe_container
FILE_LIST_THREAD_SAFE_CONTAINER=./thread_safe_container_test.cpp
thread_safe_container_test:$(FILE_LIST_THREAD_SAFE_CONTAINER)
	g++ $(FILE_LIST_THREAD_SAFE_CONTAINER) -o thread_safe_container_test $(LIB_OPTION_BASE) $(LIB_OPTION_THREAD_SAFE_CONTAINER)

LIB_OPTION_PACKET=-I../base/packet/
FILE_LIST_PACKET=./packet_test.cpp ../base/packet/packet.cpp ../protobuf/cpp/*
packet_test:$(FILE_LIST_PACKET)
	g++ $(FILE_LIST_PACKET) -o packet_test $(LIB_OPTION_BASE) $(LIB_OPTION_PACKET) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_OBJECT_POOL=-I../base/object_pool -pthread
FILE_LIST_OBJECT_POOL=./object_pool_test.cpp
object_pool_test:$(FILE_LIST_OBJECT_POOL)
	g++ $(FILE_LIST_OBJECT_POOL) -o object_pool_test $(LIB_OPTION_BASE) $(LIB_OPTION_OBJECT_POOL)

LIB_OPTION_TIMER=-I../base/timer -I../base/thread_pool -pthread
FILE_LIST_TIMER=./timer_test.cpp
timer_test:$(FILE_LIST_TIMER)
	g++ $(FILE_LIST_TIMER) -o timer_test $(LIB_OPTION_BASE) $(LIB_OPTION_TIMER)

LIB_OPTION_SHM=-I../base/shm -pthread
FILE_LIST_SHM_MASTER=./shm_master_test.cpp
FILE_LIST_SHM_SLAVE=./shm_slave_test.cpp
shm_test:shm_master_test shm_slave_test
shm_master_test:$(FILE_LIST_SHM_MASTER)
	g++ $(FILE_LIST_SHM_MASTER) -o shm_master_test $(LIB_OPTION_BASE) $(LIB_OPTION_SHM)
shm_slave_test:$(FILE_LIST_SHM_SLAVE)
	g++ $(FILE_LIST_SHM_SLAVE) -o shm_slave_test $(LIB_OPTION_BASE) $(LIB_OPTION_SHM)

LIB_OPTION_PATCH=-pthread -L. -Wl,-rpath . -fno-access-control -rdynamic -fpic -ldl -m64
FILE_LIST_PATCH=./patch_test.cpp ./patch_wind.cpp
patch_test:$(FILE_LIST_PATCH)
	g++ $(FILE_LIST_PATCH) -o patch_test $(LIB_OPTION_BASE) $(LIB_OPTION_PATCH)

LIB_OPTION_REDIS=-I../base/redis -I../third_party_lib/hiredis -L../third_party_lib/hiredis -Wl,-rpath ../third_party_lib/hiredis -lhiredis -pthread
FILE_LIST_REDIS=./redis_test.cpp ../base/redis/redis_manager.cpp
redis_test:$(FILE_LIST_REDIS)
	g++ $(FILE_LIST_REDIS) -o redis_test $(LIB_OPTION_BASE) $(LIB_OPTION_REDIS)

LIB_OPTION_CIRCLE_BUFFER=-I../base/packet -I../base/circle_buffer -I../base/net
FILE_LIST_CIRCLE_BUFFER=./circle_buffer_test.cpp ../base/packet/packet.cpp ../base/game_command.cpp ../base/circle_buffer/circle_buffer.cpp ../base/net/session.cpp ../base/net/net_manager.cpp ../protobuf/cpp/* ../protobuf/proto/protobuf_factory.cpp
circle_buffer_test:$(FILE_LIST_CIRCLE_BUFFER)
	g++ $(FILE_LIST_CIRCLE_BUFFER) -o circle_buffer_test $(LIB_OPTION_BASE) $(LIB_OPTION_CIRCLE_BUFFER) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_NET_SERVER=-I../base/packet -I../base/circle_buffer -I../base/net -pthread
LIB_OPTION_NET_CLIENT=-I../base/packet
FILE_LIST_NET_SERVER=./net_server_test.cpp ../base/packet/packet.cpp ../base/game_command.cpp ../base/circle_buffer/circle_buffer.cpp ../base/net/session.cpp ../base/net/net_manager.cpp ../protobuf/cpp/* ../protobuf/proto/protobuf_factory.cpp
FILE_LIST_NET_CLIENT=./net_client_test.cpp ../base/packet/packet.cpp ../protobuf/cpp/*
net_test:net_server_test net_client_test
net_server_test:$(FILE_LIST_NET_SERVER)
	g++ $(FILE_LIST_NET_SERVER) -o net_server_test $(LIB_OPTION_BASE) $(LIB_OPTION_NET_SERVER) $(LIB_OPTION_PROTOBUF)
net_client_test:$(FILE_LIST_NET_CLIENT)
	g++ $(FILE_LIST_NET_CLIENT) -o net_client_test $(LIB_OPTION_BASE) $(LIB_OPTION_NET_CLIENT) $(LIB_OPTION_PROTOBUF)

LIB_OPTION_SNOW_FLAKE=
FILE_LIST_SNOW_FLAKE=./snow_flake_test.cpp
snow_flake_test:$(FILE_LIST_SNOW_FLAKE)
	g++ $(FILE_LIST_SNOW_FLAKE) -o snow_flake_test $(LIB_OPTION_BASE) $(LIB_OPTION_SNOW_FLAKE)

LIB_OPTION_REDIS_LOCK=-I../base/redis -I../third_party_lib/hiredis -L../third_party_lib/hiredis -Wl,-rpath ../third_party_lib/hiredis -lhiredis -pthread
FILE_LIST_REDIS_LOCK=./redis_lock_test.cpp ../base/redis/redis_lock.cpp ../base/redis/redis_manager.cpp
redis_lock_test:$(FILE_LIST_REDIS_LOCK)
	g++ $(FILE_LIST_REDIS_LOCK) -o redis_lock_test $(LIB_OPTION_BASE) $(LIB_OPTION_REDIS_LOCK)

LIB_OPTION_REDIS_SCRIPT=-I../base/redis -I../third_party_lib/hiredis -L../third_party_lib/hiredis -Wl,-rpath ../third_party_lib/hiredis -lhiredis -pthread -DUSE_EVALSHA
FILE_LIST_REDIS_SCRIPT=./redis_script_test.cpp ../base/redis/redis_script_manager.cpp ../base/redis/redis_manager.cpp
redis_script_test:$(FILE_LIST_REDIS_SCRIPT)
	g++ $(FILE_LIST_REDIS_SCRIPT) -o redis_script_test $(LIB_OPTION_BASE) $(LIB_OPTION_REDIS_SCRIPT)

.PHONY:clean
clean:
	rm -f common_test
	rm -f log4cplus_test
	rm -f singleton_test
	rm -f thread_pool_test
	rm -f protobuf_test
	rm -f thread_safe_container_test
	rm -f packet_test
	rm -f object_pool_test
	rm -f timer_test
	rm -f shm_master_test
	rm -f shm_slave_test
	rm -f patch_test
	rm -f libpatch.so
	rm -f redis_test
	rm -f circle_buffer_test
	rm -f net_server_test
	rm -f net_client_test
	rm -f snow_flake_test
	rm -f redis_lock_test
	rm -f redis_script_test
