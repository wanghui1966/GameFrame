
cd ../protobuf/
./gen_proto.sh
cd ../test/

ulimit -c unlimited
echo "core-%e-%p-%s-%t" /proc/sys/kernel/core_pattern

make clean
make all
