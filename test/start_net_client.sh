#!/bin/sh

rm -rf log/
mkdir log
ulimit -c unlimited
ulimit -n 4096

# 客户端数量
CLIENT_COUNT=10

for i in `seq $CLIENT_COUNT`
do

	nohup ./net_client_test > log/client_$i.log 2>&1 &
	echo 客户端进程$i启动完毕
	sleep 1

done
