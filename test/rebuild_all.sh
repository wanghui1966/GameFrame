
ulimit -c unlimited
echo "core-%p-%s-%t" /proc/sys/kernel/core_pattern

make clean
make all
